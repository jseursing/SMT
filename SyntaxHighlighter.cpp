#include "SyntaxHighlighter.h"

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: Constructor
//
////////////////////////////////////////////////////////////////////////////////////////
SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent) :
  QSyntaxHighlighter(parent)
{
  SyntaxRule rule;
  QTextCharFormat format;

  // Create rule for single line comments
  format.setForeground(Qt::red);
  rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
  rule.format = format;
  Rules.append(rule);

  // Create rule for numerics
  format.setForeground(Qt::darkGreen);
  rule.pattern = QRegularExpression(QStringLiteral("[0-9_]"));
  rule.format = format;
  Rules.append(rule);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: RegisterFunctions
//
////////////////////////////////////////////////////////////////////////////////////////
void SyntaxHighlighter::RegisterFunctions(std::vector<QRegularExpression> functions)
{
  SyntaxRule rule;
  QTextCharFormat format;

  for (QRegularExpression func : functions)
  {
    format.setFontWeight(QFont::Bold);
    format.setForeground(Qt::darkBlue);
    rule.format = format;
    rule.pattern = func;
    Rules.append(rule);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: RegisterParameters
//
////////////////////////////////////////////////////////////////////////////////////////
void SyntaxHighlighter::RegisterParameters(std::vector<QRegularExpression> functions)
{
  SyntaxRule rule;
  QTextCharFormat format;

  for (QRegularExpression func : functions)
  {
    format.setFontWeight(QFont::Bold);
    format.setForeground(Qt::darkGreen);
    rule.format = format;
    rule.pattern = func;
    Rules.append(rule);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: highlightBlock
//
////////////////////////////////////////////////////////////////////////////////////////
void SyntaxHighlighter::highlightBlock(const QString& text)
{
  for (const SyntaxRule& rule : qAsConst(Rules)) 
  {
    QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) 
    {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}