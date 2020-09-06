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
 
    //GUi연동시키면서 서버Gui에 메시지띄움.
    //Gui 상에서 일단 1:1 채팅을 하고 싶다.
    private ServerSocket serverSocket;
    private Socket socket;
    private ServerGui gui;
    private String msg;
 
    //사용자들의 정보를 저장하는 맵.
    private Map<String, DataOutputStream> clientsMap = new HashMap<String, DataOutputStream>();
 
    public final void setGui(ServerGui gui) {
        this.gui = gui;
    }
 
    public void setting() throws IOException {
            Collections.synchronizedMap(clientsMap); 
            serverSocket = new ServerSocket(7777);
            while (true) {
                //서버가 할일 분담, 계속 접속 받는다.
                System.out.println("서버 대기중...");
                socket = serverSocket.accept(); //계속 반복해서 사용자를 받는다.
                System.out.println(socket.getInetAddress() + "에서 접속했습니다.");
                //새로운 사용자 쓰레드 클래스 생성해서 소켓정보를 넣어준다.
                Receiver receiver = new Receiver(socket);
                receiver.start();
            }
    }
 
    public static void main(String[] args) throws IOException {
        ServerBackground serverBackground = new ServerBackground(); //ServerBackground 객체 생성
        serverBackground.setting();
    }
 
    public void addClient(String nick, DataOutputStream out) throws IOException {//맵의 내용(Client)저장
        sendMessage(nick + "님이 접속하셨습니다.");
        clientsMap.put(nick, out);
    }
 
    public void removeClient(String nick) {//맵의 내용(Client)삭제
        sendMessage(nick + "님이 나가셨습니다.");
        clientsMap.remove(nick);
    }
 
    
    public void sendMessage(String msg) {// 메시지 내용 전파
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
 
        
        public Receiver(Socket socket) throws IOException {//네트워크 처리 듣고 처리해준다.
            out = new DataOutputStream(socket.getOutputStream());
            in = new DataInputStream(socket.getInputStream());
            nick = in.readUTF();
            addClient(nick, out);
        }
 
        public void run() {
            try {// 메세지 듣기만 계속한다.
                while (in != null) {
                    msg = in.readUTF();
                    sendMessage(msg);
                    gui.appendMsg(msg);
                }
            } catch (IOException e) {
                removeClient(nick); //사용접속종료시 에러가 발생하므로 리무브 클라이언트 처리 해준다.
            }
        }
    }
}