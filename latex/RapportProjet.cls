\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{RapportProjet}[30/11/2022, Style pour rapport de projet]

\LoadClass{Polytech}

\RequirePackage{listings}

\newcommand{\@course}{\empty}
\newcommand{\@academicyear}{\empty}

\newcommand{\course}[1]{\renewcommand{\@course}{#1}}
\newcommand{\academicyear}[1]{\renewcommand{\@academicyear}{#1}}

% definition de la commande \maketitle
\renewcommand{\maketitle}{
  \suppressfloats[t]
  \par
  \begin{center}
    \if@nologo
	  \else
    \includegraphics[height=3.5cm]{\@logopolytech}
	  \fi
    \par
	  \vspace{2.5cm}

    \textcolor{bluePolytech}{\LARGE{\textbf{\@course}}}
    \par
    \vspace{1.5cm}
    \text{\ \rule{0.9\linewidth}{1pt}}
      \Huge{\textbf{{\@title}}}
      \\[10pt]
      \large{\textbf{{\@subject}}}
    \text{\ \rule{0.9\linewidth}{1pt}}
	  \par
    \vspace{4cm}

	  \Large{
	    \begin{tabular}[t]{c}%
        \@author
      \end{tabular}
    }
    \par
    \vspace{3cm}
    \textcolor{bluePolytech}{\large\@academicyear}
  \end{center}

  \thispagestyle{empty}
  \clearpage
}