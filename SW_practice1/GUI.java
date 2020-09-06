import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

class Data {   //���Ͽ��� �ҷ��� �ܾ ������ Ŭ����
	String data1;
	String data2;
	String data3;
	String data4;
	String data5;
	String data6;
}

public class GUI extends JFrame implements ActionListener // ������ ���.
{
	private static GUI gui; // gui ��ü ����.
	private JButton LogIn, Register; // ��ư ����.
	private JLabel ChatChat, idLabel, passwordLabel; // ��
	private JTextField idField, text; // �޸���
	private JPasswordField passField; //�н������ʵ�
	private JPanel p1, p2, p3, p4, p5, p6, p7, p8, p9; // �г�.
	String str = "";   //������ �о ������ ��Ʈ�� ����

	File file = new File("Log.txt");   //�о�� ����
	FileReader fr = null;
	BufferedReader br = null;

	public GUI() throws IOException {
		
		super("ChatChat"); // ��� â�� ������ ����.
		GridLayout grid = new GridLayout(6, 1);   //���� 6�ٷ� ����.
		setLayout(grid);
		idField = new JTextField();  //ID�� �Է��� �ʵ� ����
		passField = new JPasswordField();  //password�� �Է��� �ʵ� ����
		text = new JTextField();  //�ؽ�Ʈ�� ����� �ؽ�Ʈ�ʵ� ����

		try {
			fr = new FileReader(file);
			br = new BufferedReader(fr);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		

		ChatChat = new JLabel("ChatChat");  //"ChatChat"�� ����
		idLabel = new JLabel("ID");   //"ID"�� ����
		passwordLabel = new JLabel("Password");  //"Password"�� ����

		LogIn = new JButton("Log in");  //"Log in"��ư ����
		Register = new JButton("Register now!");  //ȸ������ ��ư ����

		p1 = new JPanel(new GridLayout(1, 1));//�г�1 ����
		p2 = new JPanel(new GridLayout(1, 2));//�г�2 ����
		p3 = new JPanel(new GridLayout(1, 2));//�г�3 ����
		p4 = new JPanel(new GridLayout(1, 1));//�г�4 ����
		p5 = new JPanel(new GridLayout(1, 1));//�г�5 ����
		p6 = new JPanel(new GridLayout(1, 1));//�г�6 ����

		p1.add(ChatChat); //�г�1�� ChatChat�г� �߰�

		p2.add(idLabel); //�г�2�� ID �� �߰�
		p2.add(idField); //�г�2�� ID �Է��� �ʵ� �߰� 

		p3.add(passwordLabel); //�г�3�� Password �� �߰�
		p3.add(passField);  //�г�3�� Password �Է��� �ʵ� �߰�

		p4.add(LogIn); //�г�4�� �α��� ��ư �߰�

		p5.add(Register); //�г�5�� ȸ������ ��ư �߰�

		p6.add(text); //�г�6�� �ؽ�Ʈ�� ����� �ؽ�Ʈ�ʵ� �߰�

		add(p1); //�г� �߰�
		add(p2);
		add(p3);
		add(p4);
		add(p5);
		add(p6);

		passField.addActionListener(this);
		LogIn.addActionListener(this);
		Register.addActionListener(this);

		setSize(500, 600); //GUI ũ��
		setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); //x��ư ������ ����

	}

	public static void main(String args[]) throws IOException {
		gui = new GUI();    //GUI�� ServerGui ���� ����
		new ServerGui();
	}

	@Override
	public void actionPerformed(ActionEvent e) {

		ArrayList<Data> list = new ArrayList<>();
		if (e.getActionCommand().equals("Log in")) {//Log in��ư�� ������ ��
			try {
				while ((str = br.readLine()) != null)// ���Ͽ��� ���� �о����
				{
					String tok[] = str.split("/");// '/' ���ڸ� �����ڷ� �ܾ� ����
					Data data = new Data();// ��ü ����
					data.data1 = tok[0];// ������ �ܾ� �ֱ�
					data.data2 = tok[1];
					data.data3 = tok[2];
					data.data4 = tok[3];
					data.data5 = tok[4];
					data.data6 = tok[5];
					list.add(data);// ��ü�� ����Ʈ�� �ֱ�
					if(data.data1.equals(idField.getText()) && data.data2.equals(passField.getText())){   //ID�� Password�� �����ϴ��� üũ 
						new ClientGui();  //Log in �� �Ϸ�Ǹ� ClientGui����
						setVisible(false);   //�ʱ�ȭ�� gui�� ����
						break;
					}
					else{
					}
				}
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			
		}
		if (e.getActionCommand().equals("Register now!")) { //Register now!��ư�� ������ ��
			try {
				RegisterGUI registerGui = new RegisterGUI(); //Register now!��ư�� Ŭ���ϸ� RegisterGUI����
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		}
	}
}