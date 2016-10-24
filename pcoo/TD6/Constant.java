public class Constant implements Formula {
    private double value;
    public Constant(double value){ this.value = value; }
    public String asString(){ return Double.toString(value); }
    public double asValue(){ return value; }
}
