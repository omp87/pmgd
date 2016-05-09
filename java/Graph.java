/*
 * Corresponds to the graph.h file in Jarvis.
 *
 * Notes:
 *  - Omitting Config options at the moment
 *  - need to implement index
 */

package jarvis;

public class Graph {
    private long jarvisHandle;

    public enum OpenOptions { ReadWrite, Create, ReadOnly };
    public enum IndexType { NodeIndex, EdgeIndex };

    public Graph(String db_name, OpenOptions options) throws Exception
    {
        loadGraphNative(db_name, options.ordinal());
    }

    public native long get_id(Node n);
    public native long get_id(Edge n);

    public native NodeIterator get_nodes() throws Exception;
    public native NodeIterator get_nodes(String tag) throws Exception;

    public native NodeIterator get_nodes
                (String tag, PropertyPredicate ppred, boolean reverse)
                throws Exception;

    public NodeIterator get_nodes(String tag, PropertyPredicate ppred)
                throws Exception
        { return get_nodes(tag, ppred, false); }

    public NodeIterator get_nodes(String tag, String prop_id, String val)
                throws Exception
        { return get_nodes(tag, new PropertyPredicate(prop_id, val)); }

    public NodeIterator get_nodes(String tag, String prop_id, long val)
                throws Exception
        { return get_nodes(tag, new PropertyPredicate(prop_id, val)); }

    public Node get_node(String tag, PropertyPredicate ppred)
                throws Exception
        { return get_nodes(tag, ppred).get_current(); }

    public Node get_node(String tag, String prop_id, String val)
                throws Exception
        { return get_nodes(tag, prop_id, val).get_current(); }

    public Node get_node(String tag, String prop_id, long val)
                throws Exception
        { return get_nodes(tag, prop_id, val).get_current(); }

    public native EdgeIterator get_edges() throws Exception;
    public native EdgeIterator get_edges(String tag) throws Exception;
    public native EdgeIterator get_edges(String tag,
                                         PropertyPredicate ppred,
                                         boolean reverse) throws Exception;

    public EdgeIterator get_edges(String tag, PropertyPredicate ppred)
                throws Exception
        { return get_edges(tag, ppred, false); }

    public EdgeIterator get_edges(String tag, String prop_id, String val)
                throws Exception
        { return get_edges(tag, new PropertyPredicate(prop_id, val)); }

    public EdgeIterator get_edges(String tag, String prop_id, long val)
                throws Exception
        { return get_edges(tag, new PropertyPredicate(prop_id, val)); }

    public native Node add_node(String tag) throws Exception;
    public native Edge add_edge(Node src, Node dest, String tag)
                           throws Exception;

    public native void remove(Node n) throws Exception;
    public native void remove(Edge e) throws Exception;

    // public native void create_index(IndexType index_type, String tag,
    //                                 String property_id, PropertyType ptype);

    private native void loadGraphNative(String db_name, int options);
    public void finalize() { dispose(); }
    public native void dispose();

    static {
        System.loadLibrary("jarvis-jni");
    }
}
