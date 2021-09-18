#pragma once
#include <QRegularExpression>
#include <QSyntaxHighlighter>

class SyntaxHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT

public:
  SyntaxHighlighter(QTextDocument* parent);
  void RegisterFunctions(std::vector<QRegularExpression> functions);
  void RegisterParameters(std::vector<QRegularExpression> functions);

protected:
  void highlightBlock(const QString& text) override;

private:
  struct SyntaxRule
  {
    QRegularExpression pattern;
    QTextCharFormat format;
  };

  QVector<SyntaxRule> Rules;
};

