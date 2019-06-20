import java.awt.Color;
import java.awt.Component;
import java.util.Objects;

import javax.swing.JTree;
import javax.swing.tree.DefaultTreeCellRenderer;

class HighlightTreeCellRenderer extends DefaultTreeCellRenderer {
  
	private static final long serialVersionUID = 1L;
	private static final Color ROLLOVER_ROW_COLOR = new Color(0xEF_FD_11);
	private String query; 
	private boolean rollOver=false;
	static Component c=null;

	public void setQuery(String query) {
	    this.query=query;
	 }

	@Override
	public Color getBackgroundNonSelectionColor() {
	  return rollOver ? ROLLOVER_ROW_COLOR : super.getBackgroundNonSelectionColor();
	}
	
	@Override 
	public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {
	  c= super.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);
	  rollOver = Objects.nonNull(query) && !query.isEmpty() && Objects.toString(value, "").startsWith(query);
  	  c.setBackground(getBackgroundNonSelectionColor());
      return c;
  }
}