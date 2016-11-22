import sun.reflect.generics.visitor.Visitor;

public interface Formula {
	public String asString();
	public double asValue();
	public <R> R accept(FormulaVisitor<R> formulaVisitor);
}
