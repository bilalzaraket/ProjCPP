# Makefile compilation LaTeX
# Author : Corentin Banier
# Date : 11/01/2023

LATEX = pdflatex
LATEXOPT = --shell-escape
NONSTOP = --interaction=nonstopmode

LATEXMK = latexmk
LATEXMKOPT = -pdf
CONTINUOUS = -pvc

TARGET = rapportCPP
CLS = $(shell find *.cls)
FIGURES = $(shell find img/*)

all: $(TARGET).pdf

$(TARGET).pdf: $(SOURCES_FILES) $(FIGURES) ${CLS}
	$(LATEXMK) $(LATEXMKOPT) $(CONTINUOUS) -pdflatex="$(LATEX) $(LATEXOPT) $(NONSTOP) %O %S" $(TARGET)

once:
	$(LATEXMK) $(LATEXMKOPT) -pdflatex="$(LATEX) $(LATEXOPT) %O %S" $(TARGET)

clean:
	$(LATEXMK) -C $(TARGET)
	rm -f *.pdfsync
	rm -rf *~ *.tmp *.lof *.lot *.bcf *.nsm *.nav
	rm -f *.bbl *.blg *.aux *.end *.fls *.log *.out *.fdb_latexmk
	rm -f *.nav *.snm *.vrb *.xml *.toc img/*.pdf

.PHONY: clean once all