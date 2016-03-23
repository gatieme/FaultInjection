//import java.util.regex;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

//import java.lang.*;

public class MemoryFI
{

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

    }
}
