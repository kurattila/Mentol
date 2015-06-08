#include "AutoTest.h"

#include "src/MentolLib/ICalcOperation.h"
#include "src/MentolLib/ICalcOperationsVisualizer.h"

class CalcOperationsSvgVisualizer_Tests : public QObject
{
    Q_OBJECT
public:
    CalcOperationsSvgVisualizer_Tests();

private slots:
    void SingleLineOnlyWithStartAndEnd_WillWriteToXml();

private:
    QString m_SvgXmlHeader;
    QString m_SvgXmlFooter;
    QString getSvgXmlHeader(int svgWidth, int svgHeight);
};



CalcOperationsSvgVisualizer_Tests::CalcOperationsSvgVisualizer_Tests()
{
    m_SvgXmlHeader = "<?xml version=\"1.0\"?>"
                     "<svg version=\"1.1\""
                     "  xmlns=\"http://www.w3.org/2000/svg\""
                     "  xmlns:xlink=\"http://www.w3.org/1999/xlink\""
                     "  width=\"%WIDTH%\" height=\"%HEIGHT%\""
                     "  style=\"background-color: #D2B48C;\">";
    m_SvgXmlFooter = "</svg>";
}

void CalcOperationsSvgVisualizer_Tests::SingleLineOnlyWithStartAndEnd_WillWriteToXml()
{
    std::unique_ptr<ICalcOperationsVisualizer> visualizer( CreateCalcOperationsSvgVisualizer() );
    std::list<ICalcOperation_shptr> allOps;
    std::unique_ptr<ISvgVisualizerContext> context( CreateSvgVisualizerContext(100,100) );
    QString line        ("    <line x1=\"50\" y1=\"30\" x2=\"50\" y2=\"70\" stroke=\"green\" stroke-width=\"1\"/>");
    QString startCircle ("    <circle cx=\"50\" cy=\"30\" r=\"20\" fill=\"lightgreen\" stroke=\"green\" stroke-width=\"1\"/>");
    QString startText   ("    <text x=\"50\" y=\"34\" font-size=\"15px\" style=\"text-anchor: middle;\">Start</text>");
    QString endCircle   ("    <circle cx=\"50\" cy=\"70\" r=\"20\" fill=\"lightgreen\" stroke=\"green\" stroke-width=\"1\"/>");
    QString endText     ("    <text x=\"50\" y=\"74\" font-size=\"15px\" style=\"text-anchor: middle;\">End</text>");
    QString goodXmlContent = getSvgXmlHeader(100,100) + line
                                                      + startCircle
                                                      + startText
                                                      + endCircle
                                                      + endText
                                                      + m_SvgXmlFooter;

    auto svgXmlContent = visualizer->GetResult(allOps, context.get());

    QCOMPARE(svgXmlContent, goodXmlContent);
}

QString CalcOperationsSvgVisualizer_Tests::getSvgXmlHeader(int svgWidth, int svgHeight)
{
    QString svgXmlHeader = m_SvgXmlHeader;
    svgXmlHeader.replace("%WIDTH%", QString::number(svgWidth));
    svgXmlHeader.replace("%HEIGHT%", QString::number(svgHeight));
    return svgXmlHeader;
}


#include "CalcOperationsSvgVisualizer_Tests.moc"

DECLARE_TEST(CalcOperationsSvgVisualizer_Tests)
