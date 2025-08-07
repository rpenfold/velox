"use strict";

exports.__esModule = true;
exports["default"] = func;
exports.SYMBOL = void 0;

var _number = require("./../../helper/number");

var _error = require("./../../error");

var SYMBOL = '-';
exports.SYMBOL = SYMBOL;

function func(first) {
  for (var _len = arguments.length, rest = new Array(_len > 1 ? _len - 1 : 0), _key = 1; _key < _len; _key++) {
    rest[_key - 1] = arguments[_key];
  }

  var result = rest.reduce(function (acc, value) {
    return acc - (0, _number.toNumber)(value);
  }, (0, _number.toNumber)(first));

  if (isNaN(result)) {
    throw Error(_error.ERROR_VALUE);
  }

  return result;
}

func.SYMBOL = SYMBOL;