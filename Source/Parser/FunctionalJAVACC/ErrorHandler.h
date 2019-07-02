/* Generated By:JavaCC: Do not edit this line. ErrorHandler.h Version 6.0 */
/* JavaCCOptions:STATIC=true,SUPPORT_CLASS_VISIBILITY_PUBLIC=true */
#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H
#include <string>
#include "JavaCC.h"
#include "Token.h"

namespace Konclude {
namespace Parser {
namespace FunctionalJAVACC {
class OWL2FunctionalStyleParser;
  class ErrorHandler {
    protected:
      int error_count;
    public:
      // Called when the parser encounters a different token when expecting to
      // consume a specific kind of token.
      // expectedKind - token kind that the parser was trying to consume.
      // expectedToken - the image of the token - tokenImages[expectedKind].
      // actual - the actual token that the parser got instead.
      virtual void handleUnexpectedToken(int expectedKind, JAVACC_STRING_TYPE expectedToken, Token *actual, OWL2FunctionalStyleParser *parser) {
        error_count++;
        fprintf(stderr, "Expecting %s at: %d:%d but got %s\n", addUnicodeEscapes(expectedToken).c_str(), actual->beginLine, actual->beginColumn, addUnicodeEscapes(actual->image).c_str());
      }
      // Called when the parser cannot continue parsing.
      // last - the last token successfully parsed.
      // unexpected - the token at which the error occurs.
      // production - the production in which this error occurrs.
      virtual void handleParseError(Token *last, Token *unexpected, JAVACC_SIMPLE_STRING production, OWL2FunctionalStyleParser *parser) {
        error_count++;
        fprintf(stderr, "Encountered: %s at: %d:%d while parsing: %s\n", addUnicodeEscapes(unexpected->image).c_str(), unexpected->beginLine, unexpected->beginColumn, production.c_str());
      }
      virtual int getErrorCount() {
        return error_count;
      }
      virtual ~ErrorHandler() {}
      ErrorHandler() { error_count = 0; }
  };
}
}
}

#endif
/* JavaCC - OriginalChecksum=0a367742c4600a1642742497f80bb97a (do not edit this line) */
