import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
 
public class ServerBackground {
 
    //GUi������Ű�鼭 ����Gui�� �޽������.
    //Gui �󿡼� �ϴ� 1:1 ä���� �ϰ� �ʹ�.
    private ServerSocket serverSocket;
    private Socket socket;
    private ServerGui gui;
    private String msg;
 
    //����ڵ��� ������ �����ϴ� ��.
    private Map<String, DataOutputStream> clientsMap = new HashMap<String, DataOutputStream>();
 
    public final void setGui(ServerGui gui) {
        this.gui = gui;
    }
 
    public void setting() throws IOException {
            Collections.synchronizedMap(clientsMap); 
            serverSocket = new ServerSocket(7777);
            while (true) {
                //������ ���� �д�, ��� ���� �޴´�.
                System.out.println("���� �����...");
                socket = serverSocket.accept(); //��� �ݺ��ؼ� ����ڸ� �޴´�.
                System.out.println(socket.getInetAddress() + "���� �����߽��ϴ�.");
                //���ο� ����� ������ Ŭ���� �����ؼ� ���������� �־��ش�.
                Receiver receiver = new Receiver(socket);
                receiver.start();
            }
    }
 
    public static void main(String[] args) throws IOException {
        ServerBackground serverBackground = new ServerBackground(); //ServerBackground ��ü ����
        serverBackground.setting();
    }
 
    public void addClient(String nick, DataOutputStream out) throws IOException {//���� ����(Client)����
        sendMessage(nick + "���� �����ϼ̽��ϴ�.");
        clientsMap.put(nick, out);
    }
 
    public void removeClient(String nick) {//���� ����(Client)����
        sendMessage(nick + "���� �����̽��ϴ�.");
        clientsMap.remove(nick);
    }
 
    
    public void sendMessage(String msg) {// �޽��� ���� ����
        Iterator<String> it = clientsMap.keySet().iterator();
        String key = "";
        while (it.hasNext()) {
            key = it.next();
            try {
                clientsMap.get(key).writeUTF(msg);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
 
    // -----------------------------------------------------------------------------
    class Receiver extends Thread {
        private DataInputStream in;
        private DataOutputStream out;
        private String nick;
 
        
        public Receiver(Socket socket) throws IOException {//��Ʈ��ũ ó�� ��� ó�����ش�.
            out = new DataOutputStream(socket.getOutputStream());
            in = new DataInputStream(socket.getInputStream());
            nick = in.readUTF();
            addClient(nick, out);
        }
 
        public void run() {
            try {// �޼��� ��⸸ ����Ѵ�.
                while (in != null) {
                    msg = in.readUTF();
                    sendMessage(msg);
                    gui.appendMsg(msg);
                }
            } catch (IOException e) {
                removeClient(nick); //������������ ������ �߻��ϹǷ� ������ Ŭ���̾�Ʈ ó�� ���ش�.
            }
        }
    }
}