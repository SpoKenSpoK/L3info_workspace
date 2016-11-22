public class Variable implements Formula {
	
	private String name;
	private double value;
	
	public Variable(String name, double value){
		this.name = name;
		this.value = value;
	}
	
	public String asString(){ return name; }
	public double asValue(){ return value; }

    public <R> R accept(FormulaVisitor<R> formulaVisitor) {
        return formulaVisitor.visit(this);
    }

    public void set(double value){ this.value = value; }
}
