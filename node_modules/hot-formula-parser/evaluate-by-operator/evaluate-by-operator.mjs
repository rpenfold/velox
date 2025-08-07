function _toConsumableArray(arr) { return _arrayWithoutHoles(arr) || _iterableToArray(arr) || _unsupportedIterableToArray(arr) || _nonIterableSpread(); }

function _nonIterableSpread() { throw new TypeError("Invalid attempt to spread non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method."); }

function _unsupportedIterableToArray(o, minLen) { if (!o) return; if (typeof o === "string") return _arrayLikeToArray(o, minLen); var n = Object.prototype.toString.call(o).slice(8, -1); if (n === "Object" && o.constructor) n = o.constructor.name; if (n === "Map" || n === "Set") return Array.from(o); if (n === "Arguments" || /^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(n)) return _arrayLikeToArray(o, minLen); }

function _iterableToArray(iter) { if (typeof Symbol !== "undefined" && Symbol.iterator in Object(iter)) return Array.from(iter); }

function _arrayWithoutHoles(arr) { if (Array.isArray(arr)) return _arrayLikeToArray(arr); }

function _arrayLikeToArray(arr, len) { if (len == null || len > arr.length) len = arr.length; for (var i = 0, arr2 = new Array(len); i < len; i++) { arr2[i] = arr[i]; } return arr2; }

/* eslint-disable import/no-named-as-default-member */
import add from "./operator/add.mjs";
import ampersand from "./operator/ampersand.mjs";
import divide from "./operator/divide.mjs";
import equal from "./operator/equal.mjs";
import formulaFunction from "./operator/formula-function.mjs";
import greaterThan from "./operator/greater-than.mjs";
import greaterThanOrEqual from "./operator/greater-than-or-equal.mjs";
import lessThan from "./operator/less-than.mjs";
import lessThanOrEqual from "./operator/less-than-or-equal.mjs";
import minus from "./operator/minus.mjs";
import multiply from "./operator/multiply.mjs";
import notEqual from "./operator/not-equal.mjs";
import power from "./operator/power.mjs";
import { ERROR_NAME } from "./../error.mjs";
var availableOperators = Object.create(null);
/**
 * Evaluate values by operator id.git
 *
 * @param {String} operator Operator id.
 * @param {Array} [params=[]] Arguments to evaluate.
 * @returns {*}
 */

export default function evaluateByOperator(operator) {
  var params = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : [];
  operator = operator.toUpperCase();

  if (!availableOperators[operator]) {
    throw Error(ERROR_NAME);
  }

  return availableOperators[operator].apply(availableOperators, _toConsumableArray(params));
}
/**
 * Register operator.
 *
 * @param {String|Array} symbol Symbol to register.
 * @param {Function} func Logic to register for this symbol.
 */

export function registerOperation(symbol, func) {
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
registerOperation(add.SYMBOL, add);
registerOperation(ampersand.SYMBOL, ampersand);
registerOperation(divide.SYMBOL, divide);
registerOperation(equal.SYMBOL, equal);
registerOperation(power.SYMBOL, power);
registerOperation(formulaFunction.SYMBOL, formulaFunction);
registerOperation(greaterThan.SYMBOL, greaterThan);
registerOperation(greaterThanOrEqual.SYMBOL, greaterThanOrEqual);
registerOperation(lessThan.SYMBOL, lessThan);
registerOperation(lessThanOrEqual.SYMBOL, lessThanOrEqual);
registerOperation(multiply.SYMBOL, multiply);
registerOperation(notEqual.SYMBOL, notEqual);
registerOperation(minus.SYMBOL, minus);