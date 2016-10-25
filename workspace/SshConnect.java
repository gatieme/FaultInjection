package org.hit.ballista_tools_gatieme;
import ch.ethz.ssh2.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;

public class SshConnect
{
	String apiClass_gatieme[]={"dir","env","file"};
	HashMap<String,Double> answerMap_gatieme;
	String targetClass_gatieme;
	String targetAPI_gatieme;
	String user_name_gatieme="gatieme";
	String user_password_gatieme="gatieme";
	String target_IP="192.168.10.71";
	String ans_str_gatieme="";
	Double failurerate_gatieme=0.0;
	private BufferedReader br;
    
    	private Connection conn;
   	private Session sess;
	
	
	private void testAPI()
	{
		
		try{
			conn = new Connection(target_IP);
        		conn.connect();
        		boolean isAuthenticated = conn.authenticateWithPassword(user_name_gatieme, user_password_gatieme);
        		
			if (isAuthenticated == false)
                		throw new IOException("Authentication failed.");
        			sess =conn.openSession();
            
				sess.execCommand("sudo ./memInjector -l kstack -m random -t word_flip --time 100 --timeout 3 -p 1");			
				InputStream stdout = new StreamGobbler(sess2.getStdout());
            			BufferedReader br = new BufferedReader(new InputStreamReader(stdout));	
				ans_str_gatieme=br.readLine();
            			br.close();
            			stdout.close();
            
           
			}
			else
			{
				/// NOP
			}
		
	
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		finally
		{
			sess.close();
			sess2.close();
			conn.close();
		}
		answerMap_gatieme.put(targetClass_gatieme,failurerate_zwb);
	}
	
	public RunBallista_ZWB(String targetClass_gatieme,String targetAPI_gatieme,HashMap<String, Double> answerMap_zwb){
		this.answerMap_gatieme=answerMap_gatieme;
		this.targetClass_gatieme=targetClass_gatieme;
		this.targetAPI_gatieme=targetAPI_gatieme;
	}




	/*public RunBallista_ZWB(String targetClass_gatieme, String targetAPI_gatieme,
			HashMap<String, Double> answerMap_gatieme) {
		// TODO Auto-generated constructor stub
		this.answerMap_gatieme=answerMap_gatieme;
		this.targetClass_gatieme=targetClass_gatieme;
		this.targetAPI_gatieme=targetAPI_gatieme;
		
	}*/
}
