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
            socket = new Socket("127.0.0.1", 7777); //���� ��ü ����
            System.out.println("���� �����.");
             
            out = new DataOutputStream(socket.getOutputStream()); //DataOutputStream ����
            in = new DataInputStream(socket.getInputStream());  //DataInputStream ����
             
            //�������ڸ��� �г����� �����ϸ� ������ �̰� �г������� �ν��� �ϰ� �ʿ� ����ִ´�.
            out.writeUTF(nickName); //�г��� ����.
            System.out.println("Ŭ���̾�Ʈ : �޽��� ���ۿϷ�");
            while(in!=null){
                msg=in.readUTF();
                gui.appendMsg(msg);             
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
 
    public static void main(String[] args) {
        ClientBackground clientBackground = new ClientBackground(); //ClientBackground ��ü ����
        clientBackground.connet();
    }
 
    public void sendMessage(String msg2) { //�޼����� ������.
        try {
            out.writeUTF(msg2);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
 
    public void setNickname(String nickName) { //�г��� ����
        this.nickName = nickName;
    }
 
}