$pdf_mode = 5;  # Build PDF with XeLaTeX.
$out_dir = 'build';
$aux_dir = 'build';
$xelatex = 'xelatex -synctex=1 -interaction=nonstopmode %O %S';

# Files not covered by latexmk's defaults that should also be removed by `latexmk -C`.
$clean_ext = 'synctex.gz synctex.gz(busy) run.xml tex.bak bbl bcf fdb_latexmk run tdo %R-blx.bib';
