import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Scanner;
 
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.JTextField;
 
public class ClientGui  extends JFrame implements ActionListener{
 
    private static final long serialVersionUID = 1L;
    private JTextArea jta = new JTextArea(40, 25);
    private JTextField jtf = new JTextField(25);
    private ClientBackground client = new ClientBackground();
    private static String nickName;
     
    public ClientGui() {
         
        add(jta, BorderLayout.CENTER); //가운데에 텍스트아리아 추가
        add(jtf, BorderLayout.SOUTH);  //남쪽에 텍스트필드 추가
        jtf.addActionListener(this);
         
        setDefaultCloseOperation(EXIT_ON_CLOSE); //x누르면 gui꺼짐.
        setVisible(true);
        setBounds(800, 100, 400, 600); //gui 크기
        setTitle("클라이언트"); //상당 창에 보여질 내용
         
        client.setGui(this);
        client.setNickname(nickName);
        client.connet();
    }
     
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("당신의 닉네임부터 설정하세요 : ");
        nickName = scanner.nextLine();
        scanner.close();        
         
        new ClientGui(); //ClientGui 객체 생성
         
         
    }
 
    @Override
    public void actionPerformed(ActionEvent e) {
        String msg = nickName+ ":" + jtf.getText()+"\n";
        client.sendMessage(msg); //채팅창에서 말을 치면 보내준다.
        jtf.setText(""); //텍스트필드를 초기화시킨다.
    }
 
    public void appendMsg(String msg) {
        jta.append(msg);
    }
     
 
}