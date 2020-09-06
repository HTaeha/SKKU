import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

class Data {   //파일에서 불러온 단어를 저장할 클래스
	String data1;
	String data2;
	String data3;
	String data4;
	String data5;
	String data6;
}

public class GUI extends JFrame implements ActionListener // 프레임 상속.
{
	private static GUI gui; // gui 객체 생성.
	private JButton LogIn, Register; // 버튼 생성.
	private JLabel ChatChat, idLabel, passwordLabel; // 라벨
	private JTextField idField, text; // 메모장
	private JPasswordField passField; //패스워드필드
	private JPanel p1, p2, p3, p4, p5, p6, p7, p8, p9; // 패널.
	String str = "";   //파일을 읽어서 저장할 스트링 변수

	File file = new File("Log.txt");   //읽어올 파일
	FileReader fr = null;
	BufferedReader br = null;

	public GUI() throws IOException {
		
		super("ChatChat"); // 상당 창에 보여질 내용.
		GridLayout grid = new GridLayout(6, 1);   //세로 6줄로 나눔.
		setLayout(grid);
		idField = new JTextField();  //ID를 입력할 필드 생성
		passField = new JPasswordField();  //password를 입력할 필드 생성
		text = new JTextField();  //텍스트를 띄워줄 텍스트필드 생성

		try {
			fr = new FileReader(file);
			br = new BufferedReader(fr);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		

		ChatChat = new JLabel("ChatChat");  //"ChatChat"라벨 생성
		idLabel = new JLabel("ID");   //"ID"라벨 생성
		passwordLabel = new JLabel("Password");  //"Password"라벨 생성

		LogIn = new JButton("Log in");  //"Log in"버튼 생성
		Register = new JButton("Register now!");  //회원가입 버튼 생성

		p1 = new JPanel(new GridLayout(1, 1));//패널1 생성
		p2 = new JPanel(new GridLayout(1, 2));//패널2 생성
		p3 = new JPanel(new GridLayout(1, 2));//패널3 생성
		p4 = new JPanel(new GridLayout(1, 1));//패널4 생성
		p5 = new JPanel(new GridLayout(1, 1));//패널5 생성
		p6 = new JPanel(new GridLayout(1, 1));//패널6 생성

		p1.add(ChatChat); //패널1에 ChatChat패널 추가

		p2.add(idLabel); //패널2에 ID 라벨 추가
		p2.add(idField); //패널2에 ID 입력할 필드 추가 

		p3.add(passwordLabel); //패널3에 Password 라벨 추가
		p3.add(passField);  //패널3에 Password 입력할 필드 추가

		p4.add(LogIn); //패널4에 로그인 버튼 추가

		p5.add(Register); //패널5에 회원강비 버튼 추가

		p6.add(text); //패널6에 텍스트를 띄워줄 텍스트필드 추가

		add(p1); //패널 추가
		add(p2);
		add(p3);
		add(p4);
		add(p5);
		add(p6);

		passField.addActionListener(this);
		LogIn.addActionListener(this);
		Register.addActionListener(this);

		setSize(500, 600); //GUI 크기
		setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); //x버튼 누르면 꺼짐

	}

	public static void main(String args[]) throws IOException {
		gui = new GUI();    //GUI와 ServerGui 같이 실행
		new ServerGui();
	}

	@Override
	public void actionPerformed(ActionEvent e) {

		ArrayList<Data> list = new ArrayList<>();
		if (e.getActionCommand().equals("Log in")) {//Log in버튼을 눌렀을 때
			try {
				while ((str = br.readLine()) != null)// 파일에서 한줄 읽어오기
				{
					String tok[] = str.split("/");// '/' 문자를 구분자로 단어 추출
					Data data = new Data();// 객체 생성
					data.data1 = tok[0];// 추출한 단어 넣기
					data.data2 = tok[1];
					data.data3 = tok[2];
					data.data4 = tok[3];
					data.data5 = tok[4];
					data.data6 = tok[5];
					list.add(data);// 객체를 리스트에 넣기
					if(data.data1.equals(idField.getText()) && data.data2.equals(passField.getText())){   //ID와 Password가 존재하는지 체크 
						new ClientGui();  //Log in 이 완료되면 ClientGui실행
						setVisible(false);   //초기화면 gui는 꺼짐
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
		if (e.getActionCommand().equals("Register now!")) { //Register now!버튼을 눌렀을 때
			try {
				RegisterGUI registerGui = new RegisterGUI(); //Register now!버튼을 클릭하면 RegisterGUI실행
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		}
	}
}