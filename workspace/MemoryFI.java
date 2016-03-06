//import java.util.regex;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

import java.io.InputStreamReader;
import java.io.LineNumberReader;
//import java.io.BufferedReader;
//import java.io.InputStreamReader;



public class MemoryFI
{
    static String m_injector = "sudo ../src/injector/memInjector";
    //static String m_injector = "ls";

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
    public String Inject(String location, String mode, String type, int time, int pid, String pname)
    {
        //  拼接运行字符串
        String  cmd = m_injector + " -l " + location + " -m " + mode + " -t " + type + " --time " + time  + " --timeout 3";
        if(pid != -1)
        {
            cmd += " -p " + pid;
        }
        else
        {
            cmd += " -e " + pname;
        }
        System.out.println(cmd);

        //  运行脚本
        String result = this.runCommand(cmd, -1);
        //System.out.println("RESULT : " + result);
        String code = GetResult(result);
        return code;

    }
    /*
     *  从输出的结果中获取到指令的返回值序列
     *
     *  最后的返回串为如下类似的格式
     *  [ 2016-5-24 18:39:48]Process 1 [ inject | exited | term ] sucesss with code 0(KT_RUN)
     *
     *  最后的返回码的信息如下
     *  0(KT_RUN)
     * */
    public String GetResult(String result)
    {
        System.out.println("RESULT : " + result);
        // [ 2016-5-24 18:39:48]Process 1 inject sucesss with code 0(KT_RUN)
        int pos = result.lastIndexOf(" ");
        String code = result.substring(pos + 1);        // 0(KT_RUN)

        System.out.println("code : " + code);
        return code;
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
                InputStreamReader ir = new InputStreamReader(pos.getInputStream());

                LineNumberReader input = new LineNumberReader(ir);

                String line="";

                while ((line = input.readLine()) != null)
                {
                    System.out.println(line);
                    buf.append(line + "\n");
                }
                /*
                BufferedReader read = new BufferedReader(new InputStreamReader(pos));
                String line = null;
                while((line = read.readLine()) != null)
                {
                          System.out.println(line);
                }
                rt = buf.toString();
                */
                input.close( );
                ir.close( );

                return buf.toString();
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


    public static void main(String args[])
    {
        //System.out.println("Hello");
        MemoryFI mem = new MemoryFI();
        //mem.TestRegex();

        String result = mem.Inject("stack", "random", "word_0", 3, 1, null);
        System.out.println("RESULT : " + result);
    }
}
