/**
 * Created by spoken on 22/11/16.
 */
public interface FormulaVisitor<R> {
    public R visit(Variable variable);
    public R visit(Sum sum);
    public R visit(Product product);
}
