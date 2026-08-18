$pdf_mode = 5;                          # режим XeLaTeX → PDF
$pdflatex = 'xelatex %O %S';
$xelatex  = 'xelatex -synctex=1 -interaction=nonstopmode %O %S';
$clean_ext = 'synctex.gz synctex.gz(busy) run.xml tex.bak bbl bcf fdb_latexmk run tdo %R-blx.bib';