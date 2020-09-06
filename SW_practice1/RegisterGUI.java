import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class RegisterGUI extends JFrame implements ActionListener // ������ ���.
{
	private static RegisterGUI RegisterGui; // gui ��ü ����.
	private JButton submit, cancel; // ��ư ����.
	private JLabel textLabel, nameLabel, passwdLabel, idLabel, repassLabel, phoneLabel, emailLabel, schoolLabel; // ��
	private JTextField nameField, idField, schoolField, emailField, phoneField1, phoneField2, phoneField3, text; // �ؽ�Ʈ
	private JPasswordField passField, repassField;// �н�����
	private JPanel p0,p1, p2, p3, p4, p5, p6, p7, p8, p9; // �г�.

	File file = new File("Log.txt"); //�ҷ��� ����
	FileReader fr = null;
	FileWriter fw = null;
	BufferedReader br = null;
	
	public RegisterGUI () throws IOException {
		super("Register now"); // ��� â�� ������ ����.
		GridLayout grid = new GridLayout(10, 1); //���� 10�ٷ� gui�� �����.
		setLayout(grid);
		nameField = new JTextField();  //�̸��� �Է��� �ʵ�
		idField = new JTextField();  //ID�� �Է��� �ʵ�
		schoolField = new JTextField();  //�б�,���� ������ �Է��� �ʵ�
		emailField = new JTextField();  //e-mail�� �Է��� �ʵ�
		passField = new JPasswordField();  //password�� �Է��� �ʵ�
		phoneField1 = new JTextField(); //�ڵ��� ��ȣ�� �Է��� �ʵ�
		phoneField2 = new JTextField();
		phoneField3 = new JTextField();
		repassField = new JPasswordField();  //��й�ȣ Ȯ���� �� �ʵ�
		text = new JTextField(); //�ؽ�Ʈ�� ��� �� �ؽ�Ʈ �ʵ�

		try {
			fw = new FileWriter(file, true);
			fr = new FileReader(file);
			br = new BufferedReader(fr);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		textLabel = new JLabel("*�� �ʼ��Է»����Դϴ�.");
		idLabel = new JLabel("ID    *");
		passwdLabel = new JLabel("Password    *");
		repassLabel = new JLabel("Checking password   *");
		nameLabel = new JLabel("Name");
		schoolLabel = new JLabel("school/job");
		phoneLabel = new JLabel("Phone number");
		emailLabel = new JLabel("E-mail");
		
		submit = new JButton("Submit");  //Submit ��ư ����
		cancel = new JButton("Cancel");  //Cancel ��ư ����
		
		p1 = new JPanel(new GridLayout(1,2));//�г� ����
		p2 = new JPanel(new GridLayout(1,2));
		p3 = new JPanel(new GridLayout(1,2));
		p4 = new JPanel(new GridLayout(1,2));
		p5 = new JPanel(new GridLayout(1,2));
		p6 = new JPanel(new GridLayout(1,4));
		p7 = new JPanel(new GridLayout(1,2));
		p8 = new JPanel(new GridLayout(1,1));
		p9 = new JPanel(new GridLayout(1,2));
		p0 = new JPanel();
		
		p1.add(idLabel);
		p1.add(idField);

		p2.add(passwdLabel);
		p2.add(passField);

		p3.add(repassLabel);
		p3.add(repassField);
		
		p4.add(nameLabel);
		p4.add(nameField);
		
		p5.add(schoolLabel);
		p5.add(schoolField);
		
		p6.add(phoneLabel);
		p6.add(phoneField1);
		p6.add(phoneField2);
		p6.add(phoneField3);
		
		p7.add(emailLabel);
		p7.add(emailField);
		
		p8.add(text);
		
		p0.add(textLabel);
		
		p9.add(submit);
		p9.add(cancel);
		
		add(p1); //�г� �߰�
		add(p2);
		add(p3);
		add(p4);
		add(p5);
		add(p6);
		add(p7);
		add(p8);
		add(p0);
		add(p9);
		

		passField.addActionListener(this);
		repassField.addActionListener(this);
		submit.addActionListener(this);
		cancel.addActionListener(this);

		setSize(500, 600); //GUI ũ��
		setVisible(true);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE); //x��ư�� ������ �� gui�� ����.
		
		 
	}

	public static void main(String args[]) throws IOException {
		RegisterGui = new RegisterGUI();  //RegisterGUI��ü ����
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		
		if(e.getActionCommand().equals("Submit")){ //Submit��ư�� ������ ��
			
			if(idField.getText().equals("") || passField.getText().equals("") || repassField.getText().equals("")){
				text.setText("�ʼ������� �Է����ּ���.");  //ID, Password, ��й�ȣ Ȯ��ĭ�� ��������� �ؽ�Ʈ�� ��� ��.
			}
			else if(!passField.getText().equals(repassField.getText())){//��й�ȣ�� ��й�ȣ Ȯ��ĭ�� �ٸ��� �ؽ�Ʈ�� ��� ��.
				text.setText("Password is not correct");
			}
			else{
				try {//ȸ������ �������� �ؽ�Ʈ�� ����.
						fw.write("\n"+idField.getText()+"/"+passField.getText()+"/"+nameField.getText()+"/"+schoolField.getText()
								+"/"+phoneField1.getText()+phoneField2.getText()+phoneField3.getText()+"/"+emailField.getText());
						
					
					fw.close();//FileWriter����
					fr.close();//FileReader����
					setVisible(false);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		}
		else if(e.getActionCommand().equals("Cancel")){ //Cancel��ư�� ������ ��
			setVisible(false); //GUI�� ����.
		}
	}
}