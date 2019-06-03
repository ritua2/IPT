import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;

public class ChooseGitFolder {
	static String choosertitle="Choose the git folder";

	public static void main(String[] args) {
		JFrame frame0=new JFrame("Choose Git folder");
		frame0.setBounds(100, 100, 633, 500);
		frame0.getContentPane().setLayout(null);
		JLabel jFrame3Label5 = new JLabel("Choose offline Git folder");
		jFrame3Label5.setBounds(105, 30, 300, 26);
		frame0.getContentPane().add(jFrame3Label5);
		JButton enter = new JButton("Choose");
		enter.setBounds(410, 30, 120, 30);
		enter.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				File file=new File("/");
				JFileChooser chooser= new JFileChooser();
				chooser.setDialogTitle(choosertitle);
				chooser.setFileHidingEnabled(false);
			    chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
			    chooser.setCurrentDirectory(file);
				int returnVal = chooser.showOpenDialog(chooser);
	    		if(returnVal == JFileChooser.APPROVE_OPTION) {
	    			if(chooser.getSelectedFile().exists() && chooser.getSelectedFile().isDirectory()) {
	    				if(chooser.getSelectedFile().listFiles()[0].getName().equals(".git"))
	    					new GitFileBowser().UI(chooser.getSelectedFile().getPath());
	    				else {
	    					frame0.revalidate();
							frame0.repaint();
							JLabel jFrame3Label4 = new JLabel("Error, not a git repository.");
							jFrame3Label4.setBounds(100, 100, 300, 26);
							frame0.getContentPane().add(jFrame3Label4);
							frame0.revalidate();
							frame0.repaint();
	    				}
	    			}
	    			      			   
	    		}
			}
		});
		frame0.getContentPane().add(enter);
		frame0.setVisible(true);
	}

}
