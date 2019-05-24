import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.regex.Pattern;

import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeSelectionModel;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;


public class GitFileBowser {
	private DefaultMutableTreeNode root;
	private DefaultTreeModel treeModel;
	private JTree tree;

	public void UI(String path) {
		JFrame frame = new JFrame("Git File Browser");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		File fileRoot = new File(path);
		root = new DefaultMutableTreeNode(new FileNode(fileRoot));
		treeModel = new DefaultTreeModel(root);

		tree = new JTree(treeModel);
		tree.setShowsRootHandles(true);
		tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
		tree.addTreeSelectionListener(new TreeSelectionListener() {
			public void valueChanged(TreeSelectionEvent e) {

				DefaultMutableTreeNode node = (DefaultMutableTreeNode)tree.getLastSelectedPathComponent();
				StringBuilder file1 = new StringBuilder("");
				if (node == null) return;
				Object nodeInfo = node.getUserObject();
				file1.append(fileRoot.getAbsolutePath().toString().replaceAll("\\\\", "\\\\\\\\").substring(0, fileRoot.getAbsolutePath().toString().lastIndexOf("\\")+3));
				file1.append("\\\\");
				file1.append(e.getPath().toString().replace("[", "").replace("]", "").replaceAll(", ", "\\\\\\\\"));
				JLabel jFrame3Label4 = new JLabel("Error, not a directory.");
				int flag=0;
				File file=new File(file1.toString());
				if(!file.isDirectory()) {
					frame.revalidate();
					frame.repaint();

					jFrame3Label4.setBounds(310, 40, 300, 26);
					frame.getContentPane().add(jFrame3Label4);
					frame.revalidate();
					frame.repaint();
				}else{
					if(file.listFiles().length>=3){
						for(File file2: file.listFiles()) {
							if(!file2.getName().contains(".txt")) {
								frame.revalidate();
								frame.repaint();
								jFrame3Label4 = new JLabel("Error, not the correct directory.");
								jFrame3Label4.setBounds(310, 40, 300, 26);
								frame.getContentPane().add(jFrame3Label4);
								frame.revalidate();
								frame.repaint();
								flag=1;
								break;
							}
						}
						if(flag==0)
							displayfiles(frame, tree, file);
						else
							return;
					}
				}                    
			}
		});

		JScrollPane scrollPane = new JScrollPane(tree);
		scrollPane.setPreferredSize(new Dimension(300, frame.getHeight()));

		frame.add(scrollPane, BorderLayout.WEST);
		frame.setLocationByPlatform(true);
		frame.setSize(640, 480);
		frame.setVisible(true);
		new CreateChildNodes().createChildren(fileRoot, root);
		
	}

	public void displayfiles(JFrame frame, JTree tree, File file) {

		File file1=null;
		frame.getContentPane().removeAll();
		frame.repaint();
		JPanel jPanel,jPanel2,jPanel3,jPanel4, jPanel5;
		JScrollPane jScrollPane, jScrollPane2, jScrollPane3, jScrollPane4;

		jPanel=new JPanel(new GridLayout(1,4));
		jScrollPane= new JScrollPane(tree);
		jScrollPane.setPreferredSize(new Dimension(300, frame.getHeight()));
		jPanel2= new JPanel();
		jPanel2.add(jScrollPane);
		for(File file2: file.listFiles()) {
			if(Pattern.compile(Pattern.quote("problem"), Pattern.CASE_INSENSITIVE).matcher(file2.getName()).find())
				file1=file2;

		}
		JLabel problemFile = new JLabel(readFile(file1));
		problemFile.setVerticalAlignment(JLabel.TOP);
		jScrollPane2=new JScrollPane(problemFile);
		jPanel3=new JPanel();
		jPanel3.setLayout(new BoxLayout(jPanel3, BoxLayout.Y_AXIS));
		jPanel3.add(new JLabel("Problem File"));
		jPanel3.add(jScrollPane2);

		jPanel4= new JPanel();
		jPanel4.setLayout(new BoxLayout(jPanel4, BoxLayout.Y_AXIS));
		jPanel4.add(new JLabel("Solution File"));
		for(File file2: file.listFiles()) {
			if(Pattern.compile(Pattern.quote("solution"), Pattern.CASE_INSENSITIVE).matcher(file2.getName()).find())
				file1=file2;
		}
		JLabel solutionFile = new JLabel(readFile(file1));
		solutionFile.setVerticalAlignment(JLabel.TOP);
		jScrollPane3=new JScrollPane(solutionFile);
		jPanel4.add(jScrollPane3);

		for(File file2: file.listFiles()) {
			if(Pattern.compile(Pattern.quote("explanation"), Pattern.CASE_INSENSITIVE).matcher(file2.getName()).find())
				file1=file2;

		}
		JLabel explainationFile = new JLabel(readFile(file1));
		explainationFile.setVerticalAlignment(JLabel.TOP);
		jScrollPane4=new JScrollPane(explainationFile);
		jPanel5=new JPanel();
		jPanel5.setLayout(new BoxLayout(jPanel5, BoxLayout.Y_AXIS));
		jPanel5.add(new JLabel("Explanation File"));
		jPanel5.add(jScrollPane4);
		jPanel.add(jPanel2);
		jPanel.add(jPanel3);
		jPanel.add(jPanel4);
		jPanel.add(jPanel5);
		frame.getContentPane().add(jPanel);
		frame.revalidate();
		frame.repaint();
	}

	public String readFile(File file) {
		FileInputStream inMessage;
		StringBuilder sb = new StringBuilder("<html>");
		try {
			inMessage = new FileInputStream(file);
			DataInputStream in = new DataInputStream(inMessage);
			BufferedReader br = new BufferedReader(new InputStreamReader(in));
			String strLine;
			new StringBuilder();
			//Read File Line By Line
			while ((strLine = br.readLine()) != null)   {
				sb.append(strLine+"<br/>");
			}
			sb.append("</html>");
			br.close();

		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return sb.toString();
	}
}
