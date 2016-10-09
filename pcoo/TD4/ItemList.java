public class ItemList implements ListFormat{
    private List<String> items;
    public ItemList() { items = new ArrayList<String>(); }
    public void add(String item) { items.add(item); }
    public void print(ListFormat format) {
        System.out.println(format.listStart());
        for(item i : items){
            System.out.println(format.itemStart()+i+format.itemEnd());
        }
        System.out.println(format.listEnd());
    }
}
