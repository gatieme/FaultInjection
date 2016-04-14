//import java.util.regex;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

import java.io.InputStreamReader;
import java.io.LineNumberReader;

//import java.lang.*;

public class MemoryFI
{
    static String m_injector = "sudo ./memInjector";

    /*
     *  -l --location   stack|data|text
     *  -m --mode       random | address
     *  -t --type       word_0
     *
     *
     *  pid 与 pname 二选一
     *  pid = -1 时没使用pname
     *  pname = null时，使用pid
     * */
    public void Inject(String location, String mode, String type, int pid, String pname)
    {
        //  拼接运行字符串
        String  cmd = m_injector + " -l " + location + " -m " + mode + " -t " + type;
        if(pid != -1)
        {
            cmd += " -p " + pid;
        }
        else
        {
            cmd += " -e " + pname;
        }
        System.out.println(cmd);

       // cmd = "ls -al";
        //  运行脚本
        String result = this.runCommand(cmd, -1);
        System.out.println("RESULT : " + result);
    }

    /**
    *
    *   执行shell命令
    *   String[] cmd = { "sh", "-c", "lsmod |grep linuxVmux" }或者
    *   String[] cmd = { "sh", "-c", "./load_driver.sh" }
    *   int tp = 1 返回执行结果  非1 返回命令执行后的输出
    *
    **/
    public String runCommand(String cmd, int tp)
    {

        StringBuffer buf = new StringBuffer(1000);

        String rt = "-1";

        try
        {

            Process pos = Runtime.getRuntime().exec(cmd);

            pos.waitFor();


            if(tp == 1)
            {
                if(pos.exitValue( ) == 0)
                {
                    rt = "1";
                }

            }
            else
            {
                System.out.println("start...");
                InputStreamReader ir = new InputStreamReader(pos.getInputStream());

                LineNumberReader input = new LineNumberReader(ir);

                String ln="";

                while ((ln = input.readLine()) != null)
                {

                      buf.append(ln + "\n");

                }
                /*
                 * BufferedReader read = new BufferedReader(new InputStreamReader(in));
                String line = null;
                while((line = read.readLine())!=null){
                                System.out.printlnkk(line);
                                */
                rt = buf.toString();

                input.close( );

                ir.close( );

            }

        }
        catch (java.io.IOException e)
        {

            rt = e.toString();
            System.out.println("IOException:" + rt);
        }
        catch (Exception e)
        {

            rt = e.toString();
        }


        return rt;

    }

    void Test( )
    {
        String str = "[2016-1-26 23:26:5]Process 18556 termed with signal 11(SIGSEGV)";

        //String reStr = "(/d{4}-/d{1,2}-/d{1,2} /d{1,2}:/d{1,2}:/d{1,2})]Process (/d{1,5}) (exited with code /d|termed with signal /d{1,2}/((.*?)/))";
        //String reStr = "(exited with code /d|termed with signal /d{1,2}/((.*?)/))";
        String reStr = "termed with signal ([0-9]{1,2}/((.*?)/))";
        Pattern pattern = Pattern.compile(reStr, Pattern.CASE_INSENSITIVE);
        Matcher matcher = pattern.matcher(str);

        if(matcher.matches())
        {
            System.out.println(matcher.group(0));
        }
        else
        {
            System.out.println("no data...");
        }
    }

    void TestRegex( )
    {
        // 要验证的字符串
        String str = "termed with signal 11(SIGSEGV)";
        // 正则表达式规则
        //String regEx = "termed with signal [0-9]{1,2}";
        String regEx = "/d{2}([A-Z]{7})";
        // 编译正则表达式
        Pattern pattern = Pattern.compile(regEx);
        // 忽略大小写的写法
        Matcher matcher = pattern.matcher(str);
        // 查找字符串中是否有匹配正则表达式的字符/字符串
        boolean rs = matcher.find();
        System.out.println(rs);
    }


    public static void main(String argv[])
    {
        System.out.println("Hello");
        MemoryFI mem = new MemoryFI();
        mem.TestRegex();

        mem.Inject("stack", "random", "word_0", 1, null);
    }
}
