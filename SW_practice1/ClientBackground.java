import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
 
public class ClientBackground {
 
    private Socket socket;
    private DataInputStream in;
    private DataOutputStream out;
    private ClientGui gui;
    private String msg;
    private String nickName;
 
    public final void setGui(ClientGui gui) {
        this.gui = gui;
    }
 
    public void connet() {
        try {
            socket = new Socket("127.0.0.1", 7777); //소켓 객체 생성
            System.out.println("서버 연결됨.");
             
            out = new DataOutputStream(socket.getOutputStream()); //DataOutputStream 생성
            in = new DataInputStream(socket.getInputStream());  //DataInputStream 생성
             
            //접속하자마자 닉네임을 전송하면 서버가 이걸 닉네임으로 인식을 하고 맵에 집어넣는다.
            out.writeUTF(nickName); //닉네임 전송.
            System.out.println("클라이언트 : 메시지 전송완료");
            while(in!=null){
                msg=in.readUTF();
                gui.appendMsg(msg);             
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
 
    public static void main(String[] args) {
        ClientBackground clientBackground = new ClientBackground(); //ClientBackground 객체 생성
        clientBackground.connet();
    }
 
    public void sendMessage(String msg2) { //메세지를 보낸다.
        try {
            out.writeUTF(msg2);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
 
    public void setNickname(String nickName) { //닉네임 설정
        this.nickName = nickName;
    }
 
}