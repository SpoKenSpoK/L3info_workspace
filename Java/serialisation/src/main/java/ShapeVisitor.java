public interface ShapeVisitor<R> {
    R visit(Rectangle r);
    R visit(Circle r);
}


