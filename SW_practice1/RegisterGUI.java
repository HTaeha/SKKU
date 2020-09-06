import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class RegisterGUI extends JFrame implements ActionListener // 프레임 상속.
{
	private static RegisterGUI RegisterGui; // gui 객체 생성.
	private JButton submit, cancel; // 버튼 생성.
	private JLabel textLabel, nameLabel, passwdLabel, idLabel, repassLabel, phoneLabel, emailLabel, schoolLabel; // 라벨
	private JTextField nameField, idField, schoolField, emailField, phoneField1, phoneField2, phoneField3, text; // 텍스트
	private JPasswordField passField, repassField;// 패스워드
	private JPanel p0,p1, p2, p3, p4, p5, p6, p7, p8, p9; // 패널.

	File file = new File("Log.txt"); //불러올 파일
	FileReader fr = null;
	FileWriter fw = null;
	BufferedReader br = null;
	
	public RegisterGUI () throws IOException {
		super("Register now"); // 상당 창에 보여질 내용.
		GridLayout grid = new GridLayout(10, 1); //세로 10줄로 gui를 등분함.
		setLayout(grid);
		nameField = new JTextField();  //이름을 입력할 필드
		idField = new JTextField();  //ID를 입력할 필드
		schoolField = new JTextField();  //학교,직장 정보를 입력할 필드
		emailField = new JTextField();  //e-mail을 입력할 필드
		passField = new JPasswordField();  //password를 입력할 필드
		phoneField1 = new JTextField(); //핸드폰 번호를 입력할 필드
		phoneField2 = new JTextField();
		phoneField3 = new JTextField();
		repassField = new JPasswordField();  //비밀번호 확인을 할 필드
		text = new JTextField(); //텍스트를 띄워 줄 텍스트 필드

		try {
			fw = new FileWriter(file, true);
			fr = new FileReader(file);
			br = new BufferedReader(fr);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		textLabel = new JLabel("*는 필수입력사항입니다.");
		idLabel = new JLabel("ID    *");
		passwdLabel = new JLabel("Password    *");
		repassLabel = new JLabel("Checking password   *");
		nameLabel = new JLabel("Name");
		schoolLabel = new JLabel("school/job");
		phoneLabel = new JLabel("Phone number");
		emailLabel = new JLabel("E-mail");
		
		submit = new JButton("Submit");  //Submit 버튼 생성
		cancel = new JButton("Cancel");  //Cancel 버튼 생성
		
		p1 = new JPanel(new GridLayout(1,2));//패널 생성
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
		
		add(p1); //패널 추가
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

		setSize(500, 600); //GUI 크기
		setVisible(true);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE); //x버튼을 누르면 이 gui만 꺼짐.
		
		 
	}

	public static void main(String args[]) throws IOException {
		RegisterGui = new RegisterGUI();  //RegisterGUI객체 생성
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		
		if(e.getActionCommand().equals("Submit")){ //Submit버튼을 눌렀을 떄
			
			if(idField.getText().equals("") || passField.getText().equals("") || repassField.getText().equals("")){
				text.setText("필수사항을 입력해주세요.");  //ID, Password, 비밀번호 확인칸이 비어있으면 텍스트를 띄워 줌.
			}
			else if(!passField.getText().equals(repassField.getText())){//비밀번호가 비밀번호 확인칸과 다르면 텍스트를 띄워 줌.
				text.setText("Password is not correct");
			}
			else{
				try {//회원가입 정보들을 텍스트로 저장.
						fw.write("\n"+idField.getText()+"/"+passField.getText()+"/"+nameField.getText()+"/"+schoolField.getText()
								+"/"+phoneField1.getText()+phoneField2.getText()+phoneField3.getText()+"/"+emailField.getText());
						
					
					fw.close();//FileWriter닫음
					fr.close();//FileReader닫음
					setVisible(false);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		}
		else if(e.getActionCommand().equals("Cancel")){ //Cancel버튼을 눌렀을 때
			setVisible(false); //GUI가 꺼짐.
		}
	}
}