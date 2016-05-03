/*
*  Author: HIT CS HDMC team.
*  Create: 2010-3-13 8:50:12
*  Last modified: 2010-6-16 14:08:59
*  Description:
*  Memory fault injection tool.
*
*
*  over write by gatimeme @2016-03-28
*/
#include <sys/wait.h>
#include <signal.h>

#include "common.h"
#include "memoryInjector.h"
#include "memoryEngine.h"


static int childProcess;	//for cleanup
static long inject_pa;
static long newData;
static long origData;

static int signalPid;

Injector::Injector()
{
	this->m_memoryFaultTablePath.clear();
	this->m_targetPid = -1;
	this->m_exeArguments = NULL;
}
/*

string              m_memoryFaultTablePath;
vector <memFault>   m_memoryFaultTable;

int                 m_targetPid;        //  target is an existing proce
char                **m_exeArguments;
*/

Injector::Injector(
        int                     &targetPid,          //  故障注入的进程PID
        char                    **&program,          //  故障注入的进程名
        vector<MemoryFault>     &memoryFaultTable)   //  故障注入的注入表
:   //m_targetPid(targetPid),
    //m_exeArguments(program),
    m_memoryFaultTable(memoryFaultTable)
{
    this->m_targetPid = targetPid;
	this->m_exeArguments = program;
	this->m_memoryFaultTablePath.clear();

#ifdef BUGS     // bug002
    if(m_exeArguments != NULL)
    {
        dcout <<"BUG002--exe " <<m_exeArguments[0] <<", address = " <<m_exeArguments <<endl;
/*  dcout <<"The name of the process you want to inject is \"";
    for(unsigned int i = 0; i < 5; i++)
    {
        dcout <<this->m_exeArguments[i] <<" ";
    }
    */
    }
#endif
    //this->m_targetPid = -1;
	//this->m_exeArguments = program;
}

Injector::Injector(
        int     &targetPid,
        char    **&program,
        char    *&memoryFaultTablePath)
:   //m_targetPid(targetPid),
    //m_exeArguments(program),
    m_memoryFaultTablePath(memoryFaultTablePath)
{
    this->m_targetPid = targetPid;
	this->m_exeArguments = program;
	if( this->initFaultTable() == RT_FAIL )
	{
		return ;
	}
}

Injector::~Injector()
{
	this->m_memoryFaultTable.clear();
}





// create the Injector object use the system arg...
Injector * Injector::initInjector( int argc, char **argv )
{
	Injector * pInjector = new Injector( );
	if ( pInjector == NULL)
	{
		return NULL;
	}
#ifdef DEBUG
    printf("argc = %d", argc);
    for(int i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s", i, argv[i]);
    }
#endif

    /// get arguments
	while(argc > 0)
	{
		argc--;
		argv++;

		if(strcmp(argv[0], "-c") == 0)          //  -c to set the config file...
		{
            //  you can see we read the config file in
            //  int Injector::initFaultTable( void )
            //
            //  add by gatieme @
            //  or you can use the
            //  -l --location   stack|data|text
            //  -m --mode       random | address
            //  -t --type
            //  -t --timeout
			pInjector->m_memoryFaultTablePath = argv[1];

			argv++;
		}
		else if(strcmp(argv[0], "-e") == 0)
		{
			pInjector->m_exeArguments = argv + 1;
			break;
		}
		else if(strcmp(argv[0], "-p") == 0)
		{
			pInjector->m_targetPid = atoi(argv[1]);
#ifdef DEBUG
            printf("The pid of the process you want to inject is %s == %d", argv[1], pInjector->m_targetPid);
#endif
            break;
		}
		else
		{
			printf("Unknown option: %s\n", argv[0]);
			pInjector->usage();

            delete( pInjector );

            return NULL;
		}
	}

	if( pInjector->initFaultTable() == RT_FAIL )
	{
		delete( pInjector );
		return NULL;
	}
	return pInjector;
}

int Injector::initFaultTable( void )
{
    //  eg text random word_0 1 3
    //  [0] text | data | stack
    //  [1] random
    //  [2] word_0 |
    //  [3] 1
    //  [4] 3

    string line;

    if( this->m_memoryFaultTablePath.empty() )
	{
		cerr << "Error:no existing fault table" << endl;

        return RT_FAIL;
	}

	ifstream infile;
	infile.open( this->m_memoryFaultTablePath.c_str(), ios::in );
	if( !infile )
	{
		cerr << "Error:unable to open file:" << this->m_memoryFaultTablePath << endl;

        return RT_FAIL;
	}
#ifdef DEBUG
    else
    {
        std::cout <<"open the config file \"" <<this->m_memoryFaultTablePath <<"\" success..." <<std::endl;
    }
#endif // DEBUG

    std::string strLine;
    std::string strTmp;
	MemoryFault faultTmp;

	while( getline(infile, strLine, '\n') )
	{
#ifdef DEBUG
        std::cout <<"read \"" <<strLine <<"\"" <<std::endl;
#endif // DEBUG

		//bind istream to the strLine
		istringstream stream(strLine);

		/// location: text or data
		strTmp.clear();
		stream >> strTmp;
		if( strTmp.empty() )
		{
			cerr << "Error:fault table format errno" << endl;
			return RT_FAIL;
		}
		else if( strTmp == "text" || strTmp == "TEXT" )
		{
			faultTmp.m_location = text_area;
		}
		else if( strTmp == "data" || strTmp == "DATA" )
		{
			faultTmp.m_location = data_area;
		}
		else if( strTmp == "stack" || strTmp == "STACK" )
		{
			faultTmp.m_location = stack_area;
		}
		else
		{
			cerr << "Error:undefined fault location" << endl;
			return RT_FAIL;
		}

		/// memory addr or random
		strTmp.clear();
		stream >> strTmp;
		if( strTmp.empty() )
		{
			cerr << "Error:fault table format errno" << endl;
			return RT_FAIL;
		}

		if( strTmp == "random" || strTmp == "RANDOM" )
		{
			faultTmp.m_addr = -1;
		}
		else
		{
			int iRet = sscanf( strTmp.c_str(), "%lx", &faultTmp.m_addr );

			if( iRet != 1 )
            {
                return RT_FAIL;
            }
		}

		/// fault type: one_bit_flip, etc
		strTmp.clear();
		stream >> strTmp;
		if( strTmp.empty() )
		{
			cerr << "Error:fault table format errno" << endl;

            return RT_FAIL;
		}
		else if( strTmp == "one_bit_0" )
		{
			faultTmp.m_faultType = one_bit_0;
		}
		else if( strTmp == "one_bit_1" )
		{
			faultTmp.m_faultType = one_bit_1;
		}
		else if( strTmp == "one_bit_flip" )
		{
			faultTmp.m_faultType = one_bit_flip;
		}

		else if( strTmp == "word_0" )
		{
			faultTmp.m_faultType = word_0;
		}
		else if( strTmp == "page_0" )
		{
			faultTmp.m_faultType = page_0;
		}
/*
		else if( strTmp == "two_bit_0" )
		{
			faultTmp.faultType = two_bit_0;
		}
		else if( strTmp == "two_bit_1" )
		{
			faultTmp.faultType = two_bit_1;
		}
		else if( strTmp == "two_bit_flip" )
		{
			faultTmp.faultType = two_bit_flip;
		}
		else if( strTmp == "low_8_0" )
		{
			faultTmp.faultType = low_8_0;
		}
		else if( strTmp == "low_8_1" )
		{
			faultTmp.faultType = low_8_1;
		}
		else if( strTmp == "low_8_error" )
		{
			faultTmp.faultType = low_8_error;
		}
		else if( strTmp == "word_0" )
		{
			faultTmp.faultType = word_0;
		}
		else if( strTmp == "word_1" )
		{
			faultTmp.faultType = word_1;
		}
		else if( strTmp == "word_error" )
		{
			faultTmp.faultType = word_error;
		}
		else if( strTmp == "page_0" )
		{
			faultTmp.faultType = page_0;
		}
		else if( strTmp == "page_1" )
		{
			faultTmp.faultType = page_1;
		}
		else if( strTmp == "page_error" )
		{
			faultTmp.faultType = page_error;
		}
*/
		else
		{
			cerr << "Error:undefined fault type" << endl;
			return RT_FAIL;
		}

		/// how long fault exits, not support yet
		strTmp.clear();
		stream >> strTmp;

        if( strTmp.empty() )
		{
			cerr << "Error:fault table format errno" << endl;
			return RT_FAIL;
		}
		faultTmp.m_time = atoi( strTmp.c_str() );

		/// timeout
		strTmp.clear();
		stream >> strTmp;
		if( strTmp.empty() )
		{
			cerr << "Error:fault table format errno" << endl;
			return RT_FAIL;
		}
		faultTmp.m_timeout = atoi( strTmp.c_str() );

		//  add a fault into fault vector
		this->m_memoryFaultTable.push_back( faultTmp );
	}
#ifdef DEBUG
    cout <<"list the Memory FaultTable..." <<endl;
    for(vector<MemoryFault>::iterator iter = this->m_memoryFaultTable.begin( );
        iter != this->m_memoryFaultTable.end( );
        iter++)
    {
        cout <<*iter <<endl;
    }
#endif
	infile.close();
	return RT_OK;
}

int Injector::startInjection( void )
{
	int iRet;
	int data = 0;

    //inject fault into an existing proces
    //  系统进程无法进行跟踪
	if( this->m_targetPid > 0 && this->m_targetPid <= 10 && this->m_exeArguments == NULL )
	{
        dcout <<endl <<"[" <<__FILE__  <<", "<<__LINE__ <<"]--pid = " <<this->m_targetPid <<endl;
		iRet = injectFaults( this->m_targetPid );
		if( iRet != RT_OK )
        {
            return RT_FAIL;
        }
		return RT_OK;
	}
    else if( this->m_targetPid > 0 && this->m_exeArguments == NULL )    //  用户进程需要跟踪用户的状态
	{
        dcout <<endl <<"[" <<__FILE__  <<", "<<__LINE__ <<"]--pid = " <<this->m_targetPid <<endl;

        //设置跟踪进程，等待子进程停止
		signalPid = this->m_targetPid;		//用于给sigAlrm函数传递进程号
		iRet = ptraceAttach( this->m_targetPid );
		if( iRet == RT_FAIL ) { return RT_FAIL; }

		do
        {
			iRet = procMonitor( this->m_targetPid,data );

            dcout <<__LINE__ <<endl;
            if( iRet == RT_FAIL ) { return RT_FAIL; }

            dcout <<__LINE__ <<endl;
        }while( iRet == RUN );

		// should be STOP
		if( iRet != STOP )
		{
			writeResult( this->m_targetPid, iRet, data );	//exit or term

            return RT_FAIL;
		}

		//  进行故障注入
		iRet = injectFaults( this->m_targetPid );
		if( iRet != RT_OK ) { return RT_FAIL; }

		//  继续执行
		ptraceCont( this->m_targetPid );

		//  跟踪继续执行后的子进程
		while( 1 )
		{
			iRet = procMonitor( this->m_targetPid, data );
			if( iRet == RT_FAIL )
            {
                return RT_FAIL;
            }

			if( iRet == RUN )
			{
                dcout <<"[" <<__FILE__ <<", " <<__LINE__ <<"]--The process (PID = " <<this->m_targetPid <<") is still running" <<endl;
				continue;
			}
			else if( iRet == STOP )
			{
                dcout <<"[" <<__FILE__ <<", " <<__LINE__ <<"]--The process (PID = " <<this->m_targetPid <<") is stop with singnal" <<endl;
				if( data == SIGTRAP )
                {
                    data = 0;
                }
				ptraceCont( this->m_targetPid, data );
			}
			else //exit or term
			{
                dcout <<"[" <<__FILE__ <<", " <<__LINE__ <<"]--The process (PID = " <<this->m_targetPid <<") is exit or term" <<endl;
				writeResult( this->m_targetPid, iRet, data );
				break;
			}
		}
		return RT_OK;
	}

	//inject fault into an excultable program
	if( this->m_exeArguments != NULL && this->m_targetPid < 0 )
	{

        dcout <<endl <<"["<<__FILE__  <<", "<<__LINE__ <<"]--exe = " <<*(this->m_exeArguments) <<", inject fault into an excultable program" <<endl;
		errno = 0;
		pid_t child = fork();
		if( child < 0 )
		{
			perror("fork");
			return RT_FAIL;
		}
		else if( child == 0 )	/// child
		{
            dcout <<endl <<"["<<__FILE__  <<", "<<__LINE__ <<"]--child pid = " <<getpid( ) <<endl;
#ifdef BUGS
            dcout <<endl <<"BUG002--[" <<__FILE__  <<", " <<__func__ <<", "<<__LINE__ <<"]--exe = " <<this->m_exeArguments[0] <<", address = " <<this->m_exeArguments <<endl;
#endif
			startExe();
			_exit( RT_EXIT );
		}
		else	/// parent
		{
            dcout <<endl <<"["<<__FILE__  <<", "<<__LINE__ <<"]--exe = " <<*(this->m_exeArguments) <<", pid = " <<child <<" inject fault into an excultable program" <<endl;
			childProcess = child;
            //inject fault into physical memory address
#ifdef BUGS     //  BUG_002

            dcout <<endl <<"["<<__FILE__  <<", "<<__LINE__ <<"]--" <<"start inject child process pid = " <<child <<endl;
            //exit(0);
#endif
			iRet = injectFaults(child);
			if( iRet == RT_FAIL ) { cleanup(); }

			while(1)
			{
				iRet = procMonitor( child, data );
				if(iRet == RT_FAIL)
				{
					cleanup();
					return RT_FAIL;
				}
				if( iRet == RUN )
				{
                    dcout <<"[" <<__FILE__ <<", " <<__LINE__ <<"]--The process (EXE = " <<this->m_exeArguments <<") is still running" <<endl;
					continue;
				}
				else if( iRet == STOP )
				{
                    dcout <<"[" <<__FILE__ <<", " <<__LINE__ <<"]--The process (EXE = " <<this->m_exeArguments <<") is stoped" <<endl;
					cleanup();
					break;
				}
				else /// exit or term
				{
					/// 写结果文件
					cleanup();
                    dcout <<"[" <<__FILE__ <<", " <<__LINE__ <<"]--The process is exit or term" <<endl;
					writeResult( child, iRet, data );
					break;
				}
			}

            return RT_OK;
		}
	}

	cerr << "injection target is wrong" << endl;
	return RT_FAIL;
}

//RT_FAIL,进程已经结束
//RT_OK,进程没有结束
int Injector::injectFaults( int pid )
{
	int iRet;
	unsigned int i;
	taskMMInfo procInfo;
	long start_va, end_va;
	unsigned long random_offset;

    dcout <<"There are" <<this->m_memoryFaultTable.size() <<"works will be done in table" <<endl;
	for( i = 0; i < this->m_memoryFaultTable.size( ); i++ )
	{
        dcout <<"[" <<__FILE__ <<", " <<__LINE__ <<"]--" <<this->m_memoryFaultTable[i] <<endl;

        /// location
        dcout <<"==LOCATION==" <<endl;
		if( this->m_memoryFaultTable[i].m_addr == -1 )
		{
			/// get proc information
			bzero(&procInfo, sizeof(procInfo));
            dcout <<"[" <<__FILE__ <<", " <<__LINE__ <<"]--" <<"bzero taskMMInfo success" <<endl;
			iRet = getTaskInfo(pid, &procInfo);
            dcout <<"[" <<__FILE__ <<", " <<__LINE__ <<"]--" <<"get taskMMInfo success" <<endl;

#ifdef BUGS     //  BUG_3 found in 2016-05-12 13:27 by gatieme@HIT
			//  debug add by gatieme
            //  the address we get is wrong.
            //  the all point to the same virtual address which is not does not belong to the process PID
            //  so when we inject fault in this process we get "an process 8318 termed with signal 11(SIGSEGV)"
            //  SIGSEGV--Signal Segmentation Violation  https://en.wikipedia.org/wiki/Segmentation_fault
			printf("code : [%lx, %lx]\n", procInfo.start_code, procInfo.end_code);
			printf("data : [%lx, %lx]\n", procInfo.start_data, procInfo.end_data);
			printf(" brk : [%lx, %lx]\n", procInfo.start_brk, procInfo.brk);
			printf("stack: %lx\n", procInfo.start_stack);
#endif
			//debug
			if(iRet == FAIL)
			{
				cleanup();
				return RT_FAIL;
			}

			if( this->m_memoryFaultTable[i].m_location == text_area )       //  text segment
			{

				start_va = procInfo.start_code;
				end_va = procInfo.end_code;

                // add by gatieme @2016-01-23
                dprintf("[%s, %d] %d --Inject TEXT segment, [%lx, %lx]\n", __FILE__, __LINE__, iRet, start_va, end_va);
            }
			else if( this->m_memoryFaultTable[i].m_location == data_area )  //  data segment
			{
				start_va = procInfo.start_data;
				end_va = procInfo.end_data;

                dprintf("[%s, %d] %d --Inject DATA segment, [%lx, %lx]\n", __FILE__, __LINE__, iRet, start_va, end_va);
			}
			else if( this->m_memoryFaultTable[i].m_location == stack_area ) // stack segment
			{
				start_va = procInfo.start_stack - STACK_SIZE;
				end_va = procInfo.start_stack;

                dprintf("[%s, %d] %d --Inject STACK segment, [%lx, %lx]\n", __FILE__, __LINE__, iRet, start_va, end_va);
			}

			/* add by gatieme
			//printf("inject_va:\t%lx\n", start_va); //add by gatieme,
			inject_pa = virt_to_phys(pid, start_va);
			//printf("inject_pa:\t%lx\n", inject_pa); //add by gatieme,

			printf("inject_va:\t%lx\n", end_va); //add by gatieme,
			inject_pa = virt_to_phys(pid, end_va);
			//printf("inject_pa:\t%lx\n", inject_pa); //add by gatieme,

			srand( time( NULL ) );
			random_offset = rand() % (end_va - start_va);
			printf("inject_va:\t%lx\n", start_va + random_offset); //add by gatieme,
			inject_pa = virt_to_phys(pid, start_va + random_offset);
			printf("inject_pa:\t%lx\n", inject_pa); //add by gatieme,
			//return 0;
            */
			/// random addr
			srand( time( NULL ) );
			if(end_va == start_va)
				random_offset = 0;
			else
				random_offset = rand() % (end_va - start_va);

            //  Convert the virtual address [start_va + random_offset] to physics address [inject_pa]...
            inject_pa = virt_to_phys(pid, start_va + random_offset);

            dprintf("[%s, %d]--Start  Virtual Address = 0x%lx\n", __FILE__, __LINE__, start_va);
            dprintf("[%s, %d]--End    Virtual Address = 0x%lx\n", __FILE__, __LINE__, end_va);
            dprintf("[%s, %d]--Inject Virtual Address = 0x%lx\n", __FILE__, __LINE__, start_va + random_offset);
			dprintf("[%s, %d]--Inject Physics Address = 0x%lx\n", __FILE__, __LINE__, inject_pa);
		}
		else
		{
			inject_pa = this->m_memoryFaultTable[i].m_addr;
			dprintf("[%s, %d]--Inject Physics Address = 0x%lx\n", __FILE__, __LINE__, inject_pa);
		}

        if(inject_pa == -1)
        {
			dprintf("ERROR : [%s, %d]--Inject Physics Address = 0x%lx\n", __FILE__, __LINE__, inject_pa);
            return RT_FAIL;
        }

		dprintf("[%s, %d]--Inject fault at virtual:0x%lx--(physical:0x%lx)\n", __FILE__, __LINE__, start_va + random_offset, inject_pa);

		if(iRet == FAIL)
        {
            dprintf("Error [%s, %d]--iRet =%d\n", __FILE__, __LINE__, iRet);
            return RT_FAIL;
        }



        dprintf("[%s, %d]--iRet =%d, fauletype = %d\n", __FILE__, __LINE__, iRet, this->m_memoryFaultTable[i].m_faultType);
		switch( this->m_memoryFaultTable[i].m_faultType )
		{

			case one_bit_0:

                iRet = read_phy_mem(inject_pa, &origData);
				if(iRet == FAIL) { return RT_FAIL; }

                srand( time( NULL ) );
				newData = origData & ( ~(0x0000000000000001L << (rand()%16)) );

                iRet = write_phy_mem(inject_pa, &newData, sizeof(newData));
				if(iRet == FAIL) { return RT_FAIL; }

                iRet = read_phy_mem(inject_pa, &newData);
				if(iRet == FAIL) { return RT_FAIL; }

                printf("one bit 0(0x%lx -> 0x%lx)\n", origData, newData);
				break;

            case one_bit_1:
				iRet = read_phy_mem(inject_pa, &origData);
				if(iRet == FAIL) { return RT_FAIL; }

                srand( time( NULL ) );
				newData = origData | (0x0000000000000001L << (rand()%16));
				iRet = write_phy_mem(inject_pa, &newData, sizeof(newData));
				if(iRet == FAIL) { return RT_FAIL; }

                iRet = read_phy_mem(inject_pa, &newData);
				if(iRet == FAIL) { return RT_FAIL; }

                printf("one bit 1(0x%lx -> 0x%lx)\n", origData, newData);
				break;

            case one_bit_flip:

				iRet = read_phy_mem(inject_pa, &origData);
				if(iRet == FAIL) { return RT_FAIL; }

                srand( time( NULL ) );
				newData = origData ^ (0x0000000000000001L << ( rand() % 16 ));
				iRet = write_phy_mem(inject_pa, &newData, sizeof(newData));
				if(iRet == FAIL) { return RT_FAIL; }

                iRet = read_phy_mem(inject_pa, &newData);
				if(iRet == FAIL) { return RT_FAIL; }

                printf("one bit flip(0x%lx -> 0x%lx)\n", origData, newData);
				break;

            case word_0:
                printf("FaultType = word_0\n");
                printf("inject_pa = 0x%lx\n", inject_pa);

                iRet = read_phy_mem(inject_pa, &origData);
				if(iRet == FAIL)
                {
                    // modify by gatieme
                    dprintf("Error [%s, %d]--iRet = %d\n", __FILE__, __LINE__, iRet);
                    return RT_FAIL;
                }
				newData = ~(-1);            ///  for DEBUG...

#ifdef BUGS         //  this is bug_1 find in 2016-03-24 by gatieme
                printf("==BUG_1== [%s, %d]--0x%lx -=> 0x%lx\n", __FILE__, __LINE__, rigData, newData);
#endif

                iRet = write_phy_mem(inject_pa, &newData, sizeof(newData));
                if(iRet == FAIL)
                {
                    // modify by gatieme
                    dprintf("Error [%s, %d]--, iRet = %d\n", __FILE__, __LINE__, iRet);
                    return RT_FAIL;
                }

                iRet = read_phy_mem(inject_pa, &newData);
				if(iRet == FAIL)
                {
                    // modify by gatieme
                    dprintf("Error [%s, %d]--, iRet = %d\n", __FILE__, __LINE__, iRet);
                    return RT_FAIL;
                }

				printf("word 0(0x%lx -> 0x%lx)\n", origData, newData);
				break;

			case page_0:
				iRet = write_page_0(inject_pa);
				if(iRet == FAIL)
                {
                    return RT_FAIL;
                }

                dprintf("page 0\n");
				break;
			default:
				printf("Do not support yet.\n");
		}

		/// timeout to terminate child process
		timeout( this->m_memoryFaultTable[i].m_timeout, report );

	}
    dcout <<"return from " <<__func__ <<" now" <<endl;
	return RT_OK;
}

//侦测进程状态
//返回值=RT_FAIL，函数失败
//返回值=RUN，进程没有停止或结束，正在执行或等待
//返回值=STOP，进程停止
//返回值=EXIT，进程正常退出,data为退出码
//返回值=TERM，进程异常终止，data为异常信号
int Injector::procMonitor( int pid, int &data )
{
	int iRet;
	int status;
	iRet = waitpid( pid, &status, WNOHANG | WCONTINUED );
	if( iRet == -1 )
	{
		perror( "waitpid" );
		return RT_FAIL;
	}
	else if( iRet == 0 )
	{
		dbgcout <<"process " <<pid <<"is RUNning" <<endl;
		return RUN;
	}
	else if(iRet == pid)
	{
		if( WIFSTOPPED( status ) )
		{
			data = WSTOPSIG( status );
		    dbgcout  <<"process " <<pid <<"is STOPped" <<endl;
            return STOP;
		}
		else if( WIFEXITED( status ) )
		{
			data = WEXITSTATUS( status );
		    dbgcout  <<"process " <<pid <<"has been EXIT" <<endl;
			return EXIT;
		}
		else if( WIFSIGNALED( status ) )
		{
			data = WTERMSIG( status );
		    dbgcout  <<"process " <<pid <<"has ben TERM" <<endl;
			return TERM;
		}
		else
		{
            perror("unknown status");
			return RT_FAIL;
		}
	}
	else
    {
        dbgcout  <<"process " <<pid <<"isn't RUNning" <<endl;
		return RT_FAIL;
	}
}

char * Injector::nameSignal( int signo )
{
	if( signo >= SIGRTMIN && signo <= SIGRTMAX )
	{
		return "reliable signal";
	}
	switch( signo )
	{
		case 1:
			return "SIGHUP";
		case 2:
			return "SIGINT";
		case 3:
			return "SIGQUIT";
		case 4:
			return "SIGILL";
		case 5:
			return "SIGTRAP";
		case 6:
			return "SIGABRT";	//or "SIGIOT"
		case 7:
			return "SIGBUS";
		case 8:
			return "SIGFPE";
		case 9:
			return "SIGKILL";
		case 10:
			return "SIGUSR1";
		case 11:
			return "SIGSEGV";
		case 12:
			return "SIGUSR2";
		case 13:
			return "SIGPIPE";
		case 14:
			return "SIGALRM";
		case 15:
			return "SIGTERM";
		case 16:
			return "SIGSTKFLT";
		case 17:
			return "SIGCHLD";
		case 18:
			return "SIGCONT";
		case 19:
			return "SIGSTOP";
		case 20:
			return "SIGTSTP";
		case 21:
			return "SIGTTIN";
		case 22:
			return "SIGTTOU";
		case 23:
			return "SIGURG";
		case 24:
			return "SIGXCPU";
		case 25:
			return "SIGXFSZ";
		case 26:
			return "SIGVTALRM";
		case 27:
			return "SIGPROF";
		case 28:
			return "SIGWINCH";
		case 29:
			return "SIGIO"; 	// or "SIGPOLL"
		case 30:
			return "SIGPWR";
		case 31:
			return "SIGSYS";
		default:
			cerr << "unknown signal" << endl;
			return "unknown";
	}
}

void Injector::startExe()
{
	int fd;
	if((fd = open("/dev/null", O_RDWR)) == -1)
	{
		perror("open");
		_exit(RT_FAIL);
	}
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	close(fd);

    //execv( this->m_exeArguments[0], NULL);
    //execv( this->m_exeArguments[0], this->m_exeArguments);
	execv( "./top", NULL );

    // the program should not reach here, or it means error occurs during execute the ls command.
    perror("error execv : ");

}

void Injector::usage()
{
	printf("Usage:\n");
	printf("\t./memInjector -c fault.conf -e program [arguments]\n");
	printf("\t./memInjector -c fault.conf -p pid\n");
	printf("Arguments:\n");
	printf("\t1.fault description scripts.\n");
	printf("\t2.workload, workload can be a executable program or a running process ID.\n");
}

void Injector::writeResult( int pid, int status, int data )
{
	time_t localTime = time( NULL );
	tm lt = *localtime( &localTime );
	stringstream timeStamp;
	timeStamp << 1900 + lt.tm_year << "-" << 1 + lt.tm_mon << "-"	<< lt.tm_mday << " ";
	timeStamp << lt.tm_hour << ":" <<	lt.tm_min << ":" << lt.tm_sec;		//2009-10-23 16:23:12

	if( status == EXIT )
	{
		cout   << '[' << setw(19) << timeStamp.str() << ']' << "Process " << pid << " exited with code " << data << endl;
	}
	else if( status == TERM )
	{
		cout 	 << '[' << setw(19) << timeStamp.str() << ']' << "Process " << pid << " termed with signal " << data << "(" << nameSignal( data ) << ")"<< endl;
	}
}

void Injector::cleanup(void)
{
	int iRet = kill(childProcess, 0);
	if(iRet != -1)
	{
		iRet = kill(childProcess, SIGTERM);
		if(iRet < 0)
		{
			perror("kill");
			exit(RT_EXIT);
		}
	}
}

void Injector::report(int signo)
{
	int iRet;
	printf("%d timeout\n", childProcess);
	cleanup();
	iRet = write_phy_mem(inject_pa, &origData, sizeof(origData));
}
/*
void Injector::restore(int signo)
{
	int iRet;
	cout << "restore" << endl;
	iRet = write_phy_mem(inject_pa, &origData, sizeof(origData));
	kill(childProcess, SIGALRM);
}
*/
void Injector::timeout(int sec, void(*func)(int))
{
	signal(SIGALRM, func);
	alarm(sec);
}
