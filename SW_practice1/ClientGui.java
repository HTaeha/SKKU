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
         
        add(jta, BorderLayout.CENTER); //����� �ؽ�Ʈ�Ƹ��� �߰�
        add(jtf, BorderLayout.SOUTH);  //���ʿ� �ؽ�Ʈ�ʵ� �߰�
        jtf.addActionListener(this);
         
        setDefaultCloseOperation(EXIT_ON_CLOSE); //x������ gui����.
        setVisible(true);
        setBounds(800, 100, 400, 600); //gui ũ��
        setTitle("Ŭ���̾�Ʈ"); //��� â�� ������ ����
         
        client.setGui(this);
        client.setNickname(nickName);
        client.connet();
    }
     
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("����� �г��Ӻ��� �����ϼ��� : ");
        nickName = scanner.nextLine();
        scanner.close();        
         
        new ClientGui(); //ClientGui ��ü ����
         
         
    }
 
    @Override
    public void actionPerformed(ActionEvent e) {
        String msg = nickName+ ":" + jtf.getText()+"\n";
        client.sendMessage(msg); //ä��â���� ���� ġ�� �����ش�.
        jtf.setText(""); //�ؽ�Ʈ�ʵ带 �ʱ�ȭ��Ų��.
    }
 
    public void appendMsg(String msg) {
        jta.append(msg);
    }
     
 
}