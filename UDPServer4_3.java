import java.net.*;
import java.io.*;
public class UDPServer4_3{
    public static void main(String args[]){ 
    	DatagramSocket aSocket = null;
		try{
	    	aSocket = new DatagramSocket(6789);
					// create socket at agreed port
			byte[] buffer = new byte[1000];
			int i = 0;
			int count = 0; 
 			while(true){
 				DatagramPacket request = new DatagramPacket(buffer, buffer.length);
  				aSocket.receive(request); 
				
				String m = new String(request.getData());
				int j = Integer.parseInt(m.trim());
			//	System.out.println("got Message " + j);
	//If packet is out of sync then display message and resync to current message
				if (i != j) {			
					System.out.println("Datagram "+ i + " missing");
					count++;
					i = j;
				}
				i++;   
    			//DatagramPacket reply = new DatagramPacket(request.getData(), request.getLength(), 
    			//	request.getAddress(), request.getPort());
    			//aSocket.send(reply);
    		}
		}catch (SocketException e){System.out.println("Socket: " + e.getMessage());
		}catch (IOException e) {System.out.println("IO: " + e.getMessage());
		}finally {if(aSocket != null) aSocket.close();}
    }
}

