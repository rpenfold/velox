"use strict";

exports.__esModule = true;
exports["default"] = func;
exports.SYMBOL = void 0;

var _number = require("./../../helper/number");

var _error = require("./../../error");

var SYMBOL = '^';
exports.SYMBOL = SYMBOL;

function func(exp1, exp2) {
  var result = Math.pow((0, _number.toNumber)(exp1), (0, _number.toNumber)(exp2));

  if (isNaN(result)) {
    throw Error(_error.ERROR_VALUE);
  }

  return result;
}

func.SYMBOL = SYMBOL;