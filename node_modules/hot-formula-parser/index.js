"use strict";

function _typeof(obj) { "@babel/helpers - typeof"; if (typeof Symbol === "function" && typeof Symbol.iterator === "symbol") { _typeof = function _typeof(obj) { return typeof obj; }; } else { _typeof = function _typeof(obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; }; } return _typeof(obj); }

exports.__esModule = true;

var _parser = _interopRequireDefault(require("./parser"));

exports.Parser = _parser["default"];

var _supportedFormulas = _interopRequireDefault(require("./supported-formulas"));

exports.SUPPORTED_FORMULAS = _supportedFormulas["default"];

var _error = _interopRequireWildcard(require("./error"));

exports.error = _error["default"];
exports.ERROR = _error.ERROR;
exports.ERROR_DIV_ZERO = _error.ERROR_DIV_ZERO;
exports.ERROR_NAME = _error.ERROR_NAME;
exports.ERROR_NOT_AVAILABLE = _error.ERROR_NOT_AVAILABLE;
exports.ERROR_NULL = _error.ERROR_NULL;
exports.ERROR_NUM = _error.ERROR_NUM;
exports.ERROR_REF = _error.ERROR_REF;
exports.ERROR_VALUE = _error.ERROR_VALUE;

var _cell = require("./helper/cell");

exports.extractLabel = _cell.extractLabel;
exports.toLabel = _cell.toLabel;
exports.columnIndexToLabel = _cell.columnIndexToLabel;
exports.columnLabelToIndex = _cell.columnLabelToIndex;
exports.rowIndexToLabel = _cell.rowIndexToLabel;
exports.rowLabelToIndex = _cell.rowLabelToIndex;

function _getRequireWildcardCache() { if (typeof WeakMap !== "function") return null; var cache = new WeakMap(); _getRequireWildcardCache = function _getRequireWildcardCache() { return cache; }; return cache; }

function _interopRequireWildcard(obj) { if (obj && obj.__esModule) { return obj; } if (obj === null || _typeof(obj) !== "object" && typeof obj !== "function") { return { "default": obj }; } var cache = _getRequireWildcardCache(); if (cache && cache.has(obj)) { return cache.get(obj); } var newObj = {}; var hasPropertyDescriptor = Object.defineProperty && Object.getOwnPropertyDescriptor; for (var key in obj) { if (Object.prototype.hasOwnProperty.call(obj, key)) { var desc = hasPropertyDescriptor ? Object.getOwnPropertyDescriptor(obj, key) : null; if (desc && (desc.get || desc.set)) { Object.defineProperty(newObj, key, desc); } else { newObj[key] = obj[key]; } } } newObj["default"] = obj; if (cache) { cache.set(obj, newObj); } return newObj; }

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { "default": obj }; }