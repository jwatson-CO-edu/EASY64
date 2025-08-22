program temperature( output );

{ Program to convert temperatures from

 Fahrenheit to Celsius. }

const
	MIN = 32;
	MAX = 50;

	CONVERT = 5 / 9;

var
	fahren: integer;
	celsius: real;

begin
	writeln( 'Fahrenheit     Celsius' );
	writeln( '----------     -------' );

	for fahren := MIN to MAX do begin

		celsius := CONVERT * (fahren - 32);

		writeln( fahren: 10, celsius: 12: 2 );

	end;

end.

