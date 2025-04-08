program copy(input, output);

{ This program copies input to output. }

var

    c: char;

begin

    while not eof do begin

      while not eoln do begin

          read(c);

          write(c)

      end;

      readln;

      writeln

    end

end. { copy }

