import java.io.File;
import javax.swing.tree.DefaultMutableTreeNode;

public class CreateChildNodes {

	public void createChildren(File fileRoot, DefaultMutableTreeNode node) {
        File[] files = fileRoot.listFiles();
        //List<File> list=new ArrayList<File>();
        if (files == null)
        	//return null;
        	return;

        for (File file : files) {
        	DefaultMutableTreeNode childNode = new DefaultMutableTreeNode(new FileNode(file));
            node.add(childNode);
            if (file.isDirectory()) {
            	createChildren(file, childNode);
           }
        }
   // return list;    
    }

}
