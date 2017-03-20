lireEtValider() 
entier $x;
{
  faire {
   $x = lire();
		 si $x < 0 | $x = 0 alors {
		  ecrire( 99 );
		 } 
   }tantque $x < 0 | $x = 0;
  retour $x;
}

main() 
entier $val;
{
  $val = lireEtValider();
  ecrire( $val );
}
