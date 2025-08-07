"use strict";

exports.__esModule = true;
exports["default"] = func;
exports.SYMBOL = void 0;

var _formulajs = _interopRequireDefault(require("@handsontable/formulajs"));

var _supportedFormulas = _interopRequireDefault(require("./../../supported-formulas"));

var _error = require("./../../error");

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { "default": obj }; }

var SYMBOL = _supportedFormulas["default"];
exports.SYMBOL = SYMBOL;

function func(symbol) {
  return function __formulaFunction() {
    symbol = symbol.toUpperCase();
    var symbolParts = symbol.split('.');
    var foundFormula = false;
    var result;

    if (symbolParts.length === 1) {
      if (_formulajs["default"][symbolParts[0]]) {
        foundFormula = true;
        result = _formulajs["default"][symbolParts[0]].apply(_formulajs["default"], arguments);
      }
    } else {
      var length = symbolParts.length;
      var index = 0;
      var nestedFormula = _formulajs["default"];

      while (index < length) {
        nestedFormula = nestedFormula[symbolParts[index]];
        index++;

        if (!nestedFormula) {
          nestedFormula = null;
          break;
        }
      }

      if (nestedFormula) {
        foundFormula = true;
        result = nestedFormula.apply(void 0, arguments);
      }
    }

    if (!foundFormula) {
      throw Error(_error.ERROR_NAME);
    }

    return result;
  };
}

func.isFactory = true;
func.SYMBOL = SYMBOL;