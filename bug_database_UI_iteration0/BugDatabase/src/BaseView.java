import java.awt.Font;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import javax.swing.JButton;
import javax.swing.JTextArea;
import javax.swing.border.EmptyBorder;
import javax.swing.JFrame;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import java.util.*;

public class BaseView {

	private JFrame frame;
	static double width;
	static double height;
	static String database;
	static String host;
	static String passwd;
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
			
			database = args[0];
			host = args[1];
			if (args.length < 3) {
				passwd = "";
			}
			else {
				passwd = args[2];
			}
			
		    JFrame frame = new JFrame("List of Assignments");
		    frame.setExtendedState(JFrame.MAXIMIZED_BOTH); 
			Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
			width = screenSize.getWidth();
			height = screenSize.getHeight();
			
		    // Making list for repositories
		    Vector <String> labels = getBugName();
		    System.out.println(labels.elementAt(0));
		    JList jl = new JList(labels);
		    jl.setBounds(10, 10, 50, 100);
		    jl.setFont(new Font("Arial",Font.BOLD,(int) width/60));
		    jl.setBorder(new EmptyBorder(20, 20, 50, 50));
		    jl.setFixedCellHeight(50);
		    jl.setFixedCellWidth(100);
		    frame.add(new JScrollPane(jl));
		    
		    // Button to fetch files
		    JButton btn = new JButton("Get Files");
		    btn.setLayout(null);
		    btn.setFont(new Font("Arial",Font.BOLD,40));
		    btn.setPreferredSize(new Dimension(140,140));
		    frame.add(btn, BorderLayout.SOUTH);
		    btn.addActionListener(new ActionListener() {

		        @Override
		        public void actionPerformed(ActionEvent e) {
		        	String repo = (String) jl.getSelectedValue();
		        	
		        	if(repo==null) return;
		        	
		        	// get path for selected repository from database
		        	String path = getPath(repo);
		        	if(path==null) return;
		        	
		        	// Display files for the selected directory
		        	JFrame frame = new JFrame();
		        	frame = getFrame(path);
		        	frame.setVisible(true);
		        }
		    });
		    frame.setVisible(true);
	}
	public static Vector<String> getBugName() {
		Vector<String> names = new Vector<String>();
		try
		{  
			Class.forName("com.mysql.cj.jdbc.Driver");  
			Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/"+database+"?useUnicode=true&useJDBCCompliantTimezoneShift=true&useLegacyDatetimeCode=false&serverTimezone=UTC",host,passwd);  
			Statement stmt=con.createStatement();
			System.out.println("here");
			String query = "select name from bugCollection"; 
			ResultSet rs=stmt.executeQuery(query);  
			while(rs.next())  
				names.add(rs.getString(1));  
			con.close();  
		}
		catch(Exception e){ System.out.println(e);} 
		return names;

	}
	public static String getPath(String repo) {
		String path = null;
		try
		{  
			Class.forName("com.mysql.cj.jdbc.Driver");  
			Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/"+database+"?useUnicode=true&useJDBCCompliantTimezoneShift=true&useLegacyDatetimeCode=false&serverTimezone=UTC",host,passwd);  
			Statement stmt=con.createStatement();
			String query = "select folderPath from bugCollection where name = \""+repo+"\""; 
			ResultSet rs=stmt.executeQuery(query);  
			while(rs.next())  
				path = rs.getString(1);  
			con.close();  
		}
		catch(Exception e){ System.out.println(e);}  
			  
		
		return path;
	}
	public static JFrame getFrame(String path) {
			JFrame f = new JFrame("Compare Files");
			f.setExtendedState(JFrame.MAXIMIZED_BOTH); 
		    JPanel upperPanel = new JPanel();
		    JPanel lowerPanel = new JPanel();
		    f.getContentPane().add(upperPanel, "North");
		    f.getContentPane().add(lowerPanel, "South");	
		    
		    // Get a fixed file
		    JTextArea tareaFixed = new JTextArea(25, 50);
		    tareaFixed.setFont(new Font("Courier",Font.PLAIN,(int) width/75));
		    tareaFixed.setEditable(false);
		    JScrollPane topright = new JScrollPane(tareaFixed);
		    topright.getVerticalScrollBar().setPreferredSize(new Dimension(30, 0));
		    topright.getHorizontalScrollBar().setPreferredSize(new Dimension(0, 30));

		    // Get a Buggy file
		    JTextArea tareaBuggy = new JTextArea(25, 50);
		    tareaBuggy.setFont(new Font("Courier",Font.PLAIN,(int) width/75));
		    tareaBuggy.setEditable(false);
		    JScrollPane topleft = new JScrollPane(tareaBuggy);
		    topleft.getVerticalScrollBar().setPreferredSize(new Dimension(30, 0));
		    topleft.getHorizontalScrollBar().setPreferredSize(new Dimension(0, 30));
		    
		    upperPanel.add(topleft);
		    upperPanel.add(topright);
		    
		    // Get a explanation file
		    JTextArea tareaReadMe = new JTextArea(6,75);
		    tareaReadMe.setFont(new Font("Arial",Font.PLAIN,(int) width/75));
		    tareaReadMe.setEditable(false);
		    JScrollPane bottom = new JScrollPane(tareaReadMe);
		    bottom.getVerticalScrollBar().setPreferredSize(new Dimension(30, 0));
		    bottom.getHorizontalScrollBar().setPreferredSize(new Dimension(0, 30));
		    lowerPanel.add(bottom);
		    File file;
		    File file1;
		    try {
		    	file = new File(path + "/fixed_version.c");
		    	file1 = new File(path + "/buggy_version.c");
		    }
		    catch (Exception e) {
		    	file = new File(path + "/fixed_version.cu");
		    	file1 = new File(path + "/buggy_version.cu");
		    }
		    
		    File file2 = new File(path + "/Explanation.txt");
		    try
		    {
		    	BufferedReader input = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
		        tareaFixed.read(input, "READING FIXED FILE :-)");
		    } 
		    catch (Exception e) 
		    {
		     e.printStackTrace();
		    }
		    try
		    {
			     BufferedReader input = new BufferedReader(new InputStreamReader(new FileInputStream(file1)));
			     tareaBuggy.read(input, "READING BUGGY FILE :-)");
		    }
		    catch (Exception e)
		    {
			     e.printStackTrace();
			}
		    try 
		    {
			     BufferedReader input = new BufferedReader(new InputStreamReader(new FileInputStream(file2)));
			     tareaReadMe.read(input, "READING FILE :-)");
			} 
		    catch (Exception e)
		    {
			     e.printStackTrace();
			}
		    f.pack();
		    return f;
		
	}

	/**
	 * Create the application.
	 */
	public BaseView() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		width = screenSize.getWidth();
		height = screenSize.getHeight();
		frame.setBounds(0, 0, (int) width, (int) height);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

}
