import java.net.*;
import java.io.*;
public class UDPClient4_4{
    public static void main(String args[]){ 
		// args give message contents and destination hostname
		DatagramSocket aSocket = null;
		try {
			aSocket = new DatagramSocket();    
		//	byte [] m = args[0].getBytes();
			while(true){
				String someString = null;
				System.out.print("Enter Message to transmit('quit' to end): ");
				//someString = String.valueOf(i);
 		 		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
				try {
        				someString = br.readLine();
					if (someString.trim().equals("quit")){
						System.exit(1);
					}
     				 } catch (IOException ioe) {
      				   System.out.println("IO error trying to read message");
     				    System.exit(1);
     				 }
				byte[] m = someString.getBytes(); 
				
				InetAddress aHost = InetAddress.getByName(args[0]);
				int serverPort = 6780; 
					                                
				DatagramPacket request =
				 	new DatagramPacket(m,  m.length, aHost, serverPort);
				aSocket.send(request);			                        
				byte[] buffer = new byte[1000];
				DatagramPacket reply = new DatagramPacket(buffer, buffer.length);
				aSocket.setSoTimeout(1); 	
				aSocket.receive(reply);
				System.out.println("Reply: " + new String(reply.getData()));
			}
		}catch (SocketException e){System.out.println("Socket: " + e.getMessage());
		}catch (IOException e){System.out.println("IO: " + e.getMessage());
		}finally {if(aSocket != null) aSocket.close();}
	}		      	
}
