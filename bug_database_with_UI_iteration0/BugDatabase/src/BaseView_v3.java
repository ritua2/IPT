import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.Toolkit;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.io.InputStreamReader;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;

import javax.swing.border.EmptyBorder;
import javax.swing.BoxLayout;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreeSelectionModel;

import java.util.*;
import org.json.*;

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
		    JPanel jPanel1=new JPanel();
			jPanel1.setLayout(new BoxLayout(jPanel1, BoxLayout.Y_AXIS));
			//jPanel1.setBounds(0, 0, 30, 50);
			JPanel jPanel2=new JPanel();
			jPanel2.setLayout(new BoxLayout(jPanel2, BoxLayout.Y_AXIS));
			//jPanel2.setBounds(70, 0, 100, 150);
		    //frame.setExtendedState(JFrame.MAXIMIZED_BOTH); 
			frame.setSize(800, 600);
			frame.setResizable(true);
			Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
			width = screenSize.getWidth();
			height = screenSize.getHeight();
			
			// Making root tree for repositories
            DefaultMutableTreeNode root = new DefaultMutableTreeNode("Root");

            // Making list of programming model
            String[] programmingModels = {"MPI","OPENMP","CUDA"};

            // Making the bug tree
                for (String model : programmingModels) {
                        DefaultMutableTreeNode pm = new DefaultMutableTreeNode(model);
                Vector<String> tags = getTag(model);
                for (String tag : tags) {
                        DefaultMutableTreeNode t = new DefaultMutableTreeNode(tag);
                        Vector<String> labels = getBugName(model,tag);
                        for (String label : labels) {
                                DefaultMutableTreeNode l = new DefaultMutableTreeNode(label);
                                t.add(l);
                        }
                        pm.add(t);
                }
                root.add(pm);
                }
			
		    // Making list for repositories
		    Vector <String> labels = getBugName();
		    //System.out.println(labels.elementAt(0));
		    //JList jl = new JList(labels);
            
            JTree jl = new JTree(root);
			jl.setShowsRootHandles(true);
            jl.setRootVisible(false);
			jl.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
			//jl.setBounds(10, 10, 50, 100);
		    jl.setFont(new Font("Arial",Font.BOLD,(int) width/60));
		    //jl.setBorder(new EmptyBorder(20, 20, 50, 50));
		    //jl.setFixedCellHeight(50);
		    //jl.setFixedCellWidth(100);
		    //frame.add(new JScrollPane(jl));
		    JScrollPane jsp = new JScrollPane(jl);
		    //jsp.setPreferredSize(new Dimension(300, frame.getHeight()));
			jsp.setPreferredSize(new Dimension(200, 500));
			//jsp.setMaximumSize(new Dimension(500, 400));
			JTextField textField = new JTextField(30);
			
		    jPanel1.add(jsp);
			textField.setText("Search bug category, press Enter");
		    textField.addMouseListener(new MouseAdapter() {
		    	  @Override
		    	  public void mouseClicked(MouseEvent e) {
		    		  textField.setText("");
		    	  }
		    	});
		    textField.setFont(new Font("Courier", Font.PLAIN, 10));
			textField.addActionListener(new ActionListener(){

                public void actionPerformed(ActionEvent e){

                        ArrayList<String> bugList= new ArrayList<String>();
                        bugList = getSearchTags(textField.getText(),  database,  host, passwd);
                        for(String str: bugList){
							System.out.println(str);
							visitAllNodes(jl, str);
						}

                }});
			jPanel1.add(textField);
			//JButton jButton= new JButton("Get Related Bugs");
			//jButton.setAlignmentX(Component.CENTER_ALIGNMENT);
		    //jPanel1.add(jButton);
		    frame.add(jPanel1, BorderLayout.WEST);
			
			/*MouseListener mouseListener = new MouseAdapter() {
				public void mouseClicked(MouseEvent e) {
					if (e.getClickCount() == 2) {
						String repo = (String) jl.getSelectedValue();
		        	
						if(repo==null) return;
		        	
						// get path for selected repository from database
						String path = getPath(repo);
						if(path==null) return;
		        	
						// Display files for the selected directory
						//JFrame frame = new JFrame();
						modifyFrame(path, jPanel2, frame, database, host, passwd);
						frame.setVisible(true);
					}
				}
			};*/
			//jl.addMouseListener(mouseListener);
		    
		    // Button to fetch files
		    JButton btn = new JButton("Get Files");
		    btn.setLayout(null);
		    btn.setFont(new Font("Arial",Font.BOLD,40));
		    btn.setPreferredSize(new Dimension(140,140));
		    //frame.add(btn, BorderLayout.SOUTH);
		    /*btn.addActionListener(new ActionListener() {

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
		    });*/
			jl.addTreeSelectionListener(new TreeSelectionListener() {
				public void valueChanged(TreeSelectionEvent e) {
						//String repo = (String) jl.getSelectedValue();
						String repo = jl.getLastSelectedPathComponent().toString();
						if(repo==null) return;
		        	
						// get path for selected repository from database
						String path = getPath(repo);
						if(path==null) return;
		        	
						// Display files for the selected directory
						//JFrame frame = new JFrame();
						modifyFrame(path, jPanel1, textField, jPanel2, frame, database, host, passwd);
						frame.setVisible(true);
				}});
		    frame.setVisible(true);
	}
	public static Vector<String> getBugName() {
		Vector<String> names = new Vector<String>();
		try
		{  
			Class.forName("com.mysql.cj.jdbc.Driver");  
			Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/"+database+"?useUnicode=true&useJDBCCompliantTimezoneShift=true&useLegacyDatetimeCode=false&serverTimezone=UTC",host,passwd);  
			Statement stmt=con.createStatement();
			//System.out.println("here");
			String query = "select name from bugCollection"; 
			ResultSet rs=stmt.executeQuery(query);  
			while(rs.next())  
				names.add(rs.getString(1));  
			con.close();  
		}
		catch(Exception e){ System.out.println(e);} 
		return names;

	}
	public static Vector<String> getBugName(String model, String tag) {
        Vector<String> names = new Vector<String>();
        try
        {
                Class.forName("com.mysql.cj.jdbc.Driver");
                Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/"+database+"?useUnicode=true&useJDBCCompliantTimezoneShift=true&useLegacyDatetimeCode=false&serverTimezone=UTC",host,passwd);
                Statement stmt=con.createStatement();
                String query = "select distinct bugCollection.name from bugRecord join programmingModel on pm_id = programmingModel.id join bugCatagories on Cate_id = bugCatagories.id join bugCollection on Bug_id = bugCollection.id where programmingModel.name = \""+ model+"\" and bugCatagories.name = \"" + tag+"\"";
                ResultSet rs=stmt.executeQuery(query);
                while(rs.next())
                        names.add(rs.getString(1));
                con.close();
        }
        catch(Exception e){ System.out.println(e);}
        return names;

}
	public static Vector<String> getTag (String model) {
        Vector<String> names = new Vector<String>();
        try
        {
                Class.forName("com.mysql.cj.jdbc.Driver");
                Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/"+database+"?useUnicode=true&useJDBCCompliantTimezoneShift=true&useLegacyDatetimeCode=false&serverTimezone=UTC",host,passwd);
                Statement stmt=con.createStatement();
                String query = "select distinct bugCatagories.name from bugRecord join programmingModel on pm_id = programmingModel.id join bugCatagories on Cate_id = bugCatagories.id  where programmingModel.name =\""+ model+"\"";
                                        ResultSet rs=stmt.executeQuery(query);
                while(rs.next())
                        names.add(rs.getString(1));
                con.close();
				if(model.equals("CUDA")){
					System.out.println("Here1");
					for(String str : names){
						 System.out.println(str);
					}
				}
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
	public static JFrame getTagFrame(File file, String database, String host,String passwd) {
		
		InputStream is;
		String query=null;
		ResultSet rs=null;
		
		JSONObject obj = new JSONObject(file);
		//String tags = obj.getString("Tags");
		//String tags = "Wrong Results";
		//String tags = obj.get("Tags").toString();
		//int id=0;
		ArrayList<Integer> categoryIdList= new ArrayList<Integer>();
		ArrayList<Integer> bugIdList= new ArrayList<Integer>();
		ArrayList<String> nameList= new ArrayList<String>();
		
		//To be assessed further.
		/*JSONArray arr = obj.getJSONArray("Tags");
		for (int i = 0; i < arr.length(); i++)
		{
		    String post_id = arr.getJSONObject(i).getString("post_id");
		    ......
		}*/
		
		try
		{  	
			Class.forName("com.mysql.cj.jdbc.Driver");  
			Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/"+database+"?useUnicode=true&useJDBCCompliantTimezoneShift=true&useLegacyDatetimeCode=false&serverTimezone=UTC",host,passwd);  
			Statement stmt=con.createStatement();
			is = new FileInputStream(file);
			JSONTokener tokener = new JSONTokener(is);
	        JSONObject object = new JSONObject(tokener);
	        JSONArray tags = object.getJSONArray("Tags");
	        for (int i = 0; i < tags.length(); i++) {
	            query = "select ID from bugCatagories where name = \""+tags.get(i)+"\"";
				rs=stmt.executeQuery(query);
				while(rs.next())  
					categoryIdList.add(rs.getInt(1)); 
	        }
			
			for(int id:categoryIdList){
				query = "select Bug_id from bugRecord where Cate_id = \""+id+"\"";
				rs=stmt.executeQuery(query);
				while(rs.next())  
					bugIdList.add(rs.getInt("Bug_id"));
			}			
			for(int id:bugIdList){
				query = "select name from bugCollection where ID = \""+id+"\"";
				rs=stmt.executeQuery(query);
				while(rs.next())
					nameList.add(rs.getString(1));
			}
			
			con.close();  
		}
		catch(Exception e){ System.out.println(e);}  
		
		JFrame f = new JFrame("Get Related Bugs");
		f.setExtendedState(JFrame.MAXIMIZED_BOTH); 
	    JPanel upperPanel = new JPanel();
	    //JPanel lowerPanel = new JPanel();
	    f.getContentPane().add(upperPanel, "North");
	    //f.getContentPane().add(lowerPanel, "South");	
	    
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
	    //upperPanel.add(topright);
	    
	    // Get a explanation file
	    JTextArea tareaReadMe = new JTextArea(6,75);
	    tareaReadMe.setFont(new Font("Arial",Font.PLAIN,(int) width/75));
	    tareaReadMe.setEditable(false);
	    JScrollPane bottom = new JScrollPane(tareaReadMe);
	    bottom.getVerticalScrollBar().setPreferredSize(new Dimension(30, 0));
	    bottom.getHorizontalScrollBar().setPreferredSize(new Dimension(0, 30));
	    //lowerPanel.add(bottom);
	    
	    try
	    {	for(String str: nameList){
				tareaBuggy.append(str);
				tareaBuggy.append("\n");
			}
	    	//tareaBuggy.setText(nameList.toString());
	    } 
	    catch (Exception e) 
	    {
	     e.printStackTrace();
	    }
	    
	    f.pack();
	    return f;
	
}
	public static ArrayList<String> getSearchTags(String string, String database, String host,String passwd) {
		
		String query=null;
		ResultSet rs=null;
		
		
		int categoryId=0;
		ArrayList<Integer> bugIdList= new ArrayList<Integer>();
		ArrayList<String> nameList= new ArrayList<String>();
		
		
		
		try{  	
			Class.forName("com.mysql.cj.jdbc.Driver");  
			Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/"+database+"?useUnicode=true&useJDBCCompliantTimezoneShift=true&useLegacyDatetimeCode=false&serverTimezone=UTC",host,passwd);  
			Statement stmt=con.createStatement();
			
	        
	        query = "select ID from bugCatagories where name = \""+string+"\"";
			rs=stmt.executeQuery(query);
			while(rs.next())  
				categoryId=rs.getInt(1); 
	        
			query = "select Bug_id from bugRecord where Cate_id = \""+categoryId+"\"";
			rs=stmt.executeQuery(query);
			while(rs.next())  
				bugIdList.add(rs.getInt("Bug_id"));
						
			for(int id:bugIdList){
				query = "select name from bugCollection where ID = \""+id+"\"";
				rs=stmt.executeQuery(query);
				while(rs.next())
					nameList.add(rs.getString(1));
			}
			
			con.close();  
		}catch(Exception e){ System.out.println(e);}  
		
		JFrame f = new JFrame("Get Related Bugs");
		f.setExtendedState(JFrame.MAXIMIZED_BOTH); 
	    JPanel upperPanel = new JPanel();
	    //JPanel lowerPanel = new JPanel();
	    f.getContentPane().add(upperPanel, "North");
	    //f.getContentPane().add(lowerPanel, "South");	
	    
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
	    //upperPanel.add(topright);
	    
	    // Get a explanation file
	    JTextArea tareaReadMe = new JTextArea(6,75);
	    tareaReadMe.setFont(new Font("Arial",Font.PLAIN,(int) width/75));
	    tareaReadMe.setEditable(false);
	    JScrollPane bottom = new JScrollPane(tareaReadMe);
	    bottom.getVerticalScrollBar().setPreferredSize(new Dimension(30, 0));
	    bottom.getHorizontalScrollBar().setPreferredSize(new Dimension(0, 30));
	    //lowerPanel.add(bottom);
	    
	    try
	    {
	    	for(String str: nameList){
				tareaBuggy.append(str);
				tareaBuggy.append("\n");
			}
			//tareaBuggy.setText(nameList.toString());
	    } 
	    catch (Exception e) 
	    {
	     e.printStackTrace();
	    }
	    
	    f.pack();
	    return nameList;
	
}
	public static void modifyFrame(String path,JPanel jPanel1, JTextField jTextField, JPanel jPanel2, JFrame frame, String database, String host,String passwd) {
			jPanel2.removeAll();
			//JFrame f = new JFrame("Compare Files");
			
			JButton jButton= new JButton("Get Related Bugs");
			 //btn.setLayout(new FlowLayout());
			//btn.setLocation(loc_x, loc_y);
           // btn.setFont(new Font("Arial",Font.BOLD,40));
            //btn.setPreferredSize(new Dimension(10,10));
            
			jButton.setAlignmentX(Component.CENTER_ALIGNMENT);
			jPanel1.remove(jTextField);
			jPanel1.revalidate();
			jPanel1.repaint();
			jPanel1.add(jButton);
			jPanel1.add(jTextField);
			jPanel1.revalidate();
			jPanel1.repaint();
			jPanel2.removeAll();
			
			frame.setExtendedState(JFrame.MAXIMIZED_BOTH); 
		    JPanel upperPanel = new JPanel();
		    JPanel lowerPanel = new JPanel();
		  //frame.getContentPane().add(upperPanel, "North");
		    jPanel2.add(upperPanel);
		    //frame.getContentPane().add(lowerPanel, "South");
		    jPanel2.add(lowerPanel);
			
			int loc_x=(int)Math.round(frame.getWidth()*0.9);
		    int loc_y=(int)Math.round(frame.getHeight()*0.9);
		    
		    // Get a fixed file
		    JTextArea tareaFixed = new JTextArea(20, 40);
		    tareaFixed.setFont(new Font("Courier",Font.PLAIN,(int) width/75));
		    tareaFixed.setEditable(false);
		    JScrollPane topright = new JScrollPane(tareaFixed);
		    topright.getVerticalScrollBar().setPreferredSize(new Dimension(30, 0));
		    topright.getHorizontalScrollBar().setPreferredSize(new Dimension(0, 30));

		    // Get a Buggy file
		    JTextArea tareaBuggy = new JTextArea(20, 40);
		    tareaBuggy.setFont(new Font("Courier",Font.PLAIN,(int) width/75));
		    tareaBuggy.setEditable(false);
		    JScrollPane topleft = new JScrollPane(tareaBuggy);
		    topleft.getVerticalScrollBar().setPreferredSize(new Dimension(30, 0));
		    topleft.getHorizontalScrollBar().setPreferredSize(new Dimension(0, 30));
		    
		    upperPanel.add(topleft);
		    upperPanel.add(topright);
		    
		    // Get a explanation file
		    JTextArea tareaReadMe = new JTextArea(5,30);
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
			File json_file = new File(path + "/tag.json");
		    try
		    {
		    	BufferedReader input = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
		        tareaFixed.read(input, "READING FIXED FILE :-)");
		    } catch(FileNotFoundException fnfe){
				tareaFixed.setText("The File is Missing!!!");
			}
		    catch (Exception e) 
		    {
		     e.printStackTrace();
		    }
		    try
		    {
			     BufferedReader input = new BufferedReader(new InputStreamReader(new FileInputStream(file1)));
			     tareaBuggy.read(input, "READING BUGGY FILE :-)");
		    }catch(FileNotFoundException fnfe){
				tareaFixed.setText("The File is Missing!!!");
			}
		    catch (Exception e)
		    {
			     e.printStackTrace();
			}
		    try 
		    {
			     BufferedReader input = new BufferedReader(new InputStreamReader(new FileInputStream(file2)));
			     tareaReadMe.read(input, "READING FILE :-)");
			}catch(FileNotFoundException fnfe){
				tareaFixed.setText("The File is Missing!!!");
			} 
		    catch (Exception e)
		    {
			     e.printStackTrace();
			}
		    
			//JButton btn = new JButton("Get Related Bugs");
           
            
			//jPanel2.add(btn);
            //frame.getContentPane().add(btn, BorderLayout.SOUTH);
			jButton.addActionListener(new ActionListener() {

                @Override
                public void actionPerformed(ActionEvent e) {
                        

                        // Display files for the selected directory
                        JFrame frame = new JFrame();
                        frame = getTagFrame(json_file,database, host, passwd);
                        frame.setVisible(true);
                }
            });
			frame.getContentPane().add(jPanel2);
		    frame.pack();
		    
		
	}
	public static void visitAllNodes(TreeNode node, String str) {
		//System.out.println(node);
		if(str.equals(node.toString()))
			
		if (node.getChildCount() >= 0) {
			for (Enumeration e = node.children(); e.hasMoreElements();) {
				TreeNode n = (TreeNode) e.nextElement();
				visitAllNodes(n,str);
			}
		}
	}
	public static void visitAllNodes(JTree tree, String str) {
		System.out.println("Here");
		TreeNode root = (TreeNode) tree.getModel().getRoot();
		visitAllNodes(root, str);
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
