#include "xcoresupporter.h"

XCoreSupporter::XCoreSupporter(QObject *parent) : BaseSupporter(new BaseGrammar(parent), parent)
{

}

XCoreSupporter::~XCoreSupporter()
{
    delete interpreter;
}
