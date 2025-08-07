"use strict";

exports.__esModule = true;
exports["default"] = error;
exports.isValidStrict = isValidStrict;
exports.ERROR_VALUE = exports.ERROR_REF = exports.ERROR_NUM = exports.ERROR_NULL = exports.ERROR_NOT_AVAILABLE = exports.ERROR_NAME = exports.ERROR_DIV_ZERO = exports.ERROR = void 0;

var _errors;

function _defineProperty(obj, key, value) { if (key in obj) { Object.defineProperty(obj, key, { value: value, enumerable: true, configurable: true, writable: true }); } else { obj[key] = value; } return obj; }

var ERROR = 'ERROR';
exports.ERROR = ERROR;
var ERROR_DIV_ZERO = 'DIV/0';
exports.ERROR_DIV_ZERO = ERROR_DIV_ZERO;
var ERROR_NAME = 'NAME';
exports.ERROR_NAME = ERROR_NAME;
var ERROR_NOT_AVAILABLE = 'N/A';
exports.ERROR_NOT_AVAILABLE = ERROR_NOT_AVAILABLE;
var ERROR_NULL = 'NULL';
exports.ERROR_NULL = ERROR_NULL;
var ERROR_NUM = 'NUM';
exports.ERROR_NUM = ERROR_NUM;
var ERROR_REF = 'REF';
exports.ERROR_REF = ERROR_REF;
var ERROR_VALUE = 'VALUE';
exports.ERROR_VALUE = ERROR_VALUE;
var errors = (_errors = {}, _defineProperty(_errors, ERROR, '#ERROR!'), _defineProperty(_errors, ERROR_DIV_ZERO, '#DIV/0!'), _defineProperty(_errors, ERROR_NAME, '#NAME?'), _defineProperty(_errors, ERROR_NOT_AVAILABLE, '#N/A'), _defineProperty(_errors, ERROR_NULL, '#NULL!'), _defineProperty(_errors, ERROR_NUM, '#NUM!'), _defineProperty(_errors, ERROR_REF, '#REF!'), _defineProperty(_errors, ERROR_VALUE, '#VALUE!'), _errors);
/**
 * Return error type based on provided error id.
 *
 * @param {String} type Error type.
 * @returns {String|null} Returns error id.
 */

function error(type) {
  var result;
  type = (type + '').replace(/#|!|\?/g, '');

  if (errors[type]) {
    result = errors[type];
  }

  return result ? result : null;
}
/**
 * Check if error type is strict valid with knows errors.
 *
 * @param {String} Error type.
 * @return {Boolean}
 */


function isValidStrict(type) {
  var valid = false;

  for (var i in errors) {
    if (Object.prototype.hasOwnProperty.call(errors, i) && errors[i] === type) {
      valid = true;
      break;
    }
  }

  return valid;
}