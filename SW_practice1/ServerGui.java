import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
 
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.JTextField;
 
public class ServerGui extends JFrame implements ActionListener {
 
    private static final long serialVersionUID = 1L;
    private JTextArea jta = new JTextArea(40, 25); //텍스트아리아
    private JTextField jtf = new JTextField(25); //텍스트필드
    private ServerBackground server = new ServerBackground(); //ServerBackground객체 생성
 
    public ServerGui() throws IOException {
 
        add(jta, BorderLayout.CENTER); //가운데에 텍스트아리아 추가
        add(jtf, BorderLayout.SOUTH); //남쪽에 텍스트필드 추가
        jtf.addActionListener(this);
 
        setDefaultCloseOperation(EXIT_ON_CLOSE); //x버튼을 누르면 닫히게 함.
        setVisible(true);
        setBounds(200, 100, 400, 600); //GUI크기
        setTitle("서버부분"); //상당 창에 보여질 내용
 
        server.setGui(this);
        server.setting();
    }
 
    public static void main(String[] args) throws IOException {
        new ServerGui(); //ServerGui객체 생성
    }
 
    @Override
    public void actionPerformed(ActionEvent e) {
        String msg = "서버 : "+ jtf.getText() + "\n";
        System.out.print(msg);
        server.sendMessage(msg); //서버에 메시지를 보냄.
        jtf.setText(""); //jtf를 초기화 시킴.
    }
 
    public void appendMsg(String msg) {
        jta.append(msg);
    }
 
}