entier $a, entier $b, entier $c;

main() {
  $a = 0;
  faire {
    ecrire( $a );
    $a = $a + 1;
    $b = 0;
    faire {
      ecrire( $b );
      $b = $b + 2;
      $c = 10;
      faire {
        ecrire( $c );
        $c = $c - 1;
      } tantque 0 < $c;
      tantque $c < 10 faire {
        ecrire( $c );
        $c = $c + 1;
      }
    } tantque $b < 10;
  } tantque $a < 10;
}
