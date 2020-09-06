import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
 
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.JTextField;
 
public class ServerGui extends JFrame implements ActionListener {
 
    private static final long serialVersionUID = 1L;
    private JTextArea jta = new JTextArea(40, 25); //�ؽ�Ʈ�Ƹ���
    private JTextField jtf = new JTextField(25); //�ؽ�Ʈ�ʵ�
    private ServerBackground server = new ServerBackground(); //ServerBackground��ü ����
 
    public ServerGui() throws IOException {
 
        add(jta, BorderLayout.CENTER); //����� �ؽ�Ʈ�Ƹ��� �߰�
        add(jtf, BorderLayout.SOUTH); //���ʿ� �ؽ�Ʈ�ʵ� �߰�
        jtf.addActionListener(this);
 
        setDefaultCloseOperation(EXIT_ON_CLOSE); //x��ư�� ������ ������ ��.
        setVisible(true);
        setBounds(200, 100, 400, 600); //GUIũ��
        setTitle("�����κ�"); //��� â�� ������ ����
 
        server.setGui(this);
        server.setting();
    }
 
    public static void main(String[] args) throws IOException {
        new ServerGui(); //ServerGui��ü ����
    }
 
    @Override
    public void actionPerformed(ActionEvent e) {
        String msg = "���� : "+ jtf.getText() + "\n";
        System.out.print(msg);
        server.sendMessage(msg); //������ �޽����� ����.
        jtf.setText(""); //jtf�� �ʱ�ȭ ��Ŵ.
    }
 
    public void appendMsg(String msg) {
        jta.append(msg);
    }
 
}