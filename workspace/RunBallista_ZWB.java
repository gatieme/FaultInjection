package org.hit.ballista_tools_zwb;
import ch.ethz.ssh2.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;

public class RunBallista_ZWB{
	String apiClass_zwb[]={"dir","env","file"};
	HashMap<String,Double> answerMap_zwb;
	String targetClass_zwb;
	String targetAPI_zwb;
	String user_name_zwb="root";
	String user_password_zwb="hadoop@hoho";
	String target_IP="192.168.10.141";
	String ans_str_zwb="";
	Double failurerate_zwb=0.0d;
	private BufferedReader br;
    
    private Connection conn;
    private Session sess;
    private Session sess2;
	public void run(){
		if(targetClass_zwb.equals("all")){
			for(int i=0;i<apiClass_zwb.length;i++)
				testAPI(apiClass_zwb[i],"all");
		}
		else{
			testAPI(targetClass_zwb,targetAPI_zwb);	
		}
	
	}	
	
	
	private void testAPI(String targetClass_zwb,String targetAPI_zwb){
		
		try{
			conn = new Connection(target_IP);
        		conn.connect();
        		boolean isAuthenticated = conn.authenticateWithPassword(user_name_zwb, user_password_zwb);
        		
			if (isAuthenticated == false)
                		throw new IOException("Authentication failed.");
        			sess =conn.openSession();
            
			if(targetAPI_zwb.equals("all")){
				//connect linux system
				//sess.execCommand("rm -rf /home/hadoop/zhaoweibo/ballista/ballista/callTable.all");      //É¾³ý
				sess.execCommand("cp -f /home/hadoop/zhaoweibo/ballista/ballista/callTable."+targetClass_zwb+"  /home/hadoop/zhaoweibo/ballista/ballista/callTable.all");		//¿½±´
			
				System.out.println("run at line 51");
				sess2=conn.openSession();
				sess2.execCommand("cd /home/hadoop/zhaoweibo/ballista/ballista/ ; perl /home/hadoop/zhaoweibo/ballista/ballista/ostest.pl");	//Ö´ÐÐ
				InputStream stdout = new StreamGobbler(sess2.getStdout());
            			BufferedReader br = new BufferedReader(new InputStreamReader(stdout));	
				ans_str_zwb=br.readLine();
			
            			while(ans_str_zwb!=null){
            				System.out.println(ans_str_zwb);
					if(ans_str_zwb.trim().startsWith("OVERALL")){
						String[] split = ans_str_zwb.trim().split("\\s+");
						failurerate_zwb=Double.parseDouble(split[split.length-1].replace("%",""));
					}
					
					ans_str_zwb=br.readLine();
				}
            			br.close();
            			stdout.close();
            
           
			}
			else{
		}
		
	
		}catch(Exception e){e.printStackTrace();}
		finally{
			sess.close();
			sess2.close();
			conn.close();
		}
		answerMap_zwb.put(targetClass_zwb,failurerate_zwb);
	}
	
	public RunBallista_ZWB(String targetClass_zwb,String targetAPI_zwb,HashMap<String, Double> answerMap_zwb){
		this.answerMap_zwb=answerMap_zwb;
		this.targetClass_zwb=targetClass_zwb;
		this.targetAPI_zwb=targetAPI_zwb;
	}




	/*public RunBallista_ZWB(String targetClass_zwb, String targetAPI_zwb,
			HashMap<String, Double> answerMap_zwb) {
		// TODO Auto-generated constructor stub
		this.answerMap_zwb=answerMap_zwb;
		this.targetClass_zwb=targetClass_zwb;
		this.targetAPI_zwb=targetAPI_zwb;
		
	}*/
}
