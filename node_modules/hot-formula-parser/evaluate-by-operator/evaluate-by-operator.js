"use strict";

exports.__esModule = true;
exports["default"] = evaluateByOperator;
exports.registerOperation = registerOperation;

var _add = _interopRequireDefault(require("./operator/add"));

var _ampersand = _interopRequireDefault(require("./operator/ampersand"));

var _divide = _interopRequireDefault(require("./operator/divide"));

var _equal = _interopRequireDefault(require("./operator/equal"));

var _formulaFunction = _interopRequireDefault(require("./operator/formula-function"));

var _greaterThan = _interopRequireDefault(require("./operator/greater-than"));

var _greaterThanOrEqual = _interopRequireDefault(require("./operator/greater-than-or-equal"));

var _lessThan = _interopRequireDefault(require("./operator/less-than"));

var _lessThanOrEqual = _interopRequireDefault(require("./operator/less-than-or-equal"));

var _minus = _interopRequireDefault(require("./operator/minus"));

var _multiply = _interopRequireDefault(require("./operator/multiply"));

var _notEqual = _interopRequireDefault(require("./operator/not-equal"));

var _power = _interopRequireDefault(require("./operator/power"));

var _error = require("./../error");

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { "default": obj }; }

function _toConsumableArray(arr) { return _arrayWithoutHoles(arr) || _iterableToArray(arr) || _unsupportedIterableToArray(arr) || _nonIterableSpread(); }

function _nonIterableSpread() { throw new TypeError("Invalid attempt to spread non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method."); }

function _unsupportedIterableToArray(o, minLen) { if (!o) return; if (typeof o === "string") return _arrayLikeToArray(o, minLen); var n = Object.prototype.toString.call(o).slice(8, -1); if (n === "Object" && o.constructor) n = o.constructor.name; if (n === "Map" || n === "Set") return Array.from(o); if (n === "Arguments" || /^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(n)) return _arrayLikeToArray(o, minLen); }

function _iterableToArray(iter) { if (typeof Symbol !== "undefined" && Symbol.iterator in Object(iter)) return Array.from(iter); }

function _arrayWithoutHoles(arr) { if (Array.isArray(arr)) return _arrayLikeToArray(arr); }

function _arrayLikeToArray(arr, len) { if (len == null || len > arr.length) len = arr.length; for (var i = 0, arr2 = new Array(len); i < len; i++) { arr2[i] = arr[i]; } return arr2; }

var availableOperators = Object.create(null);
/**
 * Evaluate values by operator id.git
 *
 * @param {String} operator Operator id.
 * @param {Array} [params=[]] Arguments to evaluate.
 * @returns {*}
 */

function evaluateByOperator(operator) {
  var params = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : [];
  operator = operator.toUpperCase();

  if (!availableOperators[operator]) {
    throw Error(_error.ERROR_NAME);
  }

  return availableOperators[operator].apply(availableOperators, _toConsumableArray(params));
}
/**
 * Register operator.
 *
 * @param {String|Array} symbol Symbol to register.
 * @param {Function} func Logic to register for this symbol.
 */


function registerOperation(symbol, func) {
  if (!Array.isArray(symbol)) {
    symbol = [symbol.toUpperCase()];
  }

  symbol.forEach(function (s) {
    if (func.isFactory) {
      availableOperators[s] = func(s);
    } else {
      availableOperators[s] = func;
    }
  });
}

registerOperation(_add["default"].SYMBOL, _add["default"]);
registerOperation(_ampersand["default"].SYMBOL, _ampersand["default"]);
registerOperation(_divide["default"].SYMBOL, _divide["default"]);
registerOperation(_equal["default"].SYMBOL, _equal["default"]);
registerOperation(_power["default"].SYMBOL, _power["default"]);
registerOperation(_formulaFunction["default"].SYMBOL, _formulaFunction["default"]);
registerOperation(_greaterThan["default"].SYMBOL, _greaterThan["default"]);
registerOperation(_greaterThanOrEqual["default"].SYMBOL, _greaterThanOrEqual["default"]);
registerOperation(_lessThan["default"].SYMBOL, _lessThan["default"]);
registerOperation(_lessThanOrEqual["default"].SYMBOL, _lessThanOrEqual["default"]);
registerOperation(_multiply["default"].SYMBOL, _multiply["default"]);
registerOperation(_notEqual["default"].SYMBOL, _notEqual["default"]);
registerOperation(_minus["default"].SYMBOL, _minus["default"]);