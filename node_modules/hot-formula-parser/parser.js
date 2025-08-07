"use strict";

function _typeof(obj) { "@babel/helpers - typeof"; if (typeof Symbol === "function" && typeof Symbol.iterator === "symbol") { _typeof = function _typeof(obj) { return typeof obj; }; } else { _typeof = function _typeof(obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; }; } return _typeof(obj); }

exports.__esModule = true;
exports["default"] = void 0;

var _tinyEmitter = _interopRequireDefault(require("tiny-emitter"));

var _evaluateByOperator = _interopRequireDefault(require("./evaluate-by-operator/evaluate-by-operator"));

var _grammarParser = require("./grammar-parser/grammar-parser");

var _string = require("./helper/string");

var _number = require("./helper/number");

var _error = _interopRequireWildcard(require("./error"));

var _cell = require("./helper/cell");

function _getRequireWildcardCache() { if (typeof WeakMap !== "function") return null; var cache = new WeakMap(); _getRequireWildcardCache = function _getRequireWildcardCache() { return cache; }; return cache; }

function _interopRequireWildcard(obj) { if (obj && obj.__esModule) { return obj; } if (obj === null || _typeof(obj) !== "object" && typeof obj !== "function") { return { "default": obj }; } var cache = _getRequireWildcardCache(); if (cache && cache.has(obj)) { return cache.get(obj); } var newObj = {}; var hasPropertyDescriptor = Object.defineProperty && Object.getOwnPropertyDescriptor; for (var key in obj) { if (Object.prototype.hasOwnProperty.call(obj, key)) { var desc = hasPropertyDescriptor ? Object.getOwnPropertyDescriptor(obj, key) : null; if (desc && (desc.get || desc.set)) { Object.defineProperty(newObj, key, desc); } else { newObj[key] = obj[key]; } } } newObj["default"] = obj; if (cache) { cache.set(obj, newObj); } return newObj; }

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { "default": obj }; }

function _slicedToArray(arr, i) { return _arrayWithHoles(arr) || _iterableToArrayLimit(arr, i) || _unsupportedIterableToArray(arr, i) || _nonIterableRest(); }

function _nonIterableRest() { throw new TypeError("Invalid attempt to destructure non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method."); }

function _unsupportedIterableToArray(o, minLen) { if (!o) return; if (typeof o === "string") return _arrayLikeToArray(o, minLen); var n = Object.prototype.toString.call(o).slice(8, -1); if (n === "Object" && o.constructor) n = o.constructor.name; if (n === "Map" || n === "Set") return Array.from(o); if (n === "Arguments" || /^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(n)) return _arrayLikeToArray(o, minLen); }

function _arrayLikeToArray(arr, len) { if (len == null || len > arr.length) len = arr.length; for (var i = 0, arr2 = new Array(len); i < len; i++) { arr2[i] = arr[i]; } return arr2; }

function _iterableToArrayLimit(arr, i) { if (typeof Symbol === "undefined" || !(Symbol.iterator in Object(arr))) return; var _arr = []; var _n = true; var _d = false; var _e = undefined; try { for (var _i = arr[Symbol.iterator](), _s; !(_n = (_s = _i.next()).done); _n = true) { _arr.push(_s.value); if (i && _arr.length === i) break; } } catch (err) { _d = true; _e = err; } finally { try { if (!_n && _i["return"] != null) _i["return"](); } finally { if (_d) throw _e; } } return _arr; }

function _arrayWithHoles(arr) { if (Array.isArray(arr)) return arr; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } }

function _createClass(Constructor, protoProps, staticProps) { if (protoProps) _defineProperties(Constructor.prototype, protoProps); if (staticProps) _defineProperties(Constructor, staticProps); return Constructor; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function"); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, writable: true, configurable: true } }); if (superClass) _setPrototypeOf(subClass, superClass); }

function _setPrototypeOf(o, p) { _setPrototypeOf = Object.setPrototypeOf || function _setPrototypeOf(o, p) { o.__proto__ = p; return o; }; return _setPrototypeOf(o, p); }

function _createSuper(Derived) { var hasNativeReflectConstruct = _isNativeReflectConstruct(); return function _createSuperInternal() { var Super = _getPrototypeOf(Derived), result; if (hasNativeReflectConstruct) { var NewTarget = _getPrototypeOf(this).constructor; result = Reflect.construct(Super, arguments, NewTarget); } else { result = Super.apply(this, arguments); } return _possibleConstructorReturn(this, result); }; }

function _possibleConstructorReturn(self, call) { if (call && (_typeof(call) === "object" || typeof call === "function")) { return call; } return _assertThisInitialized(self); }

function _assertThisInitialized(self) { if (self === void 0) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return self; }

function _isNativeReflectConstruct() { if (typeof Reflect === "undefined" || !Reflect.construct) return false; if (Reflect.construct.sham) return false; if (typeof Proxy === "function") return true; try { Date.prototype.toString.call(Reflect.construct(Date, [], function () {})); return true; } catch (e) { return false; } }

function _getPrototypeOf(o) { _getPrototypeOf = Object.setPrototypeOf ? Object.getPrototypeOf : function _getPrototypeOf(o) { return o.__proto__ || Object.getPrototypeOf(o); }; return _getPrototypeOf(o); }

/**
 * @class Parser
 */
var Parser = /*#__PURE__*/function (_Emitter) {
  _inherits(Parser, _Emitter);

  var _super = _createSuper(Parser);

  function Parser() {
    var _this;

    _classCallCheck(this, Parser);

    _this = _super.call(this);
    _this.parser = new _grammarParser.Parser();
    _this.parser.yy = {
      toNumber: _number.toNumber,
      trimEdges: _string.trimEdges,
      invertNumber: _number.invertNumber,
      throwError: function throwError(errorName) {
        return _this._throwError(errorName);
      },
      callVariable: function callVariable(variable) {
        return _this._callVariable(variable);
      },
      evaluateByOperator: _evaluateByOperator["default"],
      callFunction: function callFunction(name, params) {
        return _this._callFunction(name, params);
      },
      cellValue: function cellValue(value) {
        return _this._callCellValue(value);
      },
      rangeValue: function rangeValue(start, end) {
        return _this._callRangeValue(start, end);
      }
    };
    _this.variables = Object.create(null);
    _this.functions = Object.create(null);

    _this.setVariable('TRUE', true).setVariable('FALSE', false).setVariable('NULL', null);

    return _this;
  }
  /**
   * Parse formula expression.
   *
   * @param {String} expression to parse.
   * @return {*} Returns an object with tow properties `error` and `result`.
   */


  _createClass(Parser, [{
    key: "parse",
    value: function parse(expression) {
      var result = null;
      var error = null;

      try {
        if (expression === '') {
          result = '';
        } else {
          result = this.parser.parse(expression);
        }
      } catch (ex) {
        var message = (0, _error["default"])(ex.message);

        if (message) {
          error = message;
        } else {
          error = (0, _error["default"])(_error.ERROR);
        }
      }

      if (result instanceof Error) {
        error = (0, _error["default"])(result.message) || (0, _error["default"])(_error.ERROR);
        result = null;
      }

      return {
        error: error,
        result: result
      };
    }
    /**
     * Set predefined variable name which can be visible while parsing formula expression.
     *
     * @param {String} name Variable name.
     * @param {*} value Variable value.
     * @returns {Parser}
     */

  }, {
    key: "setVariable",
    value: function setVariable(name, value) {
      this.variables[name] = value;
      return this;
    }
    /**
     * Get variable name.
     *
     * @param {String} name Variable name.
     * @returns {*}
     */

  }, {
    key: "getVariable",
    value: function getVariable(name) {
      return this.variables[name];
    }
    /**
     * Retrieve variable value by its name.
     *
     * @param name Variable name.
     * @returns {*}
     * @private
     */

  }, {
    key: "_callVariable",
    value: function _callVariable(name) {
      var value = this.getVariable(name);
      this.emit('callVariable', name, function (newValue) {
        if (newValue !== void 0) {
          value = newValue;
        }
      });

      if (value === void 0) {
        throw Error(_error.ERROR_NAME);
      }

      return value;
    }
    /**
     * Set custom function which can be visible while parsing formula expression.
     *
     * @param {String} name Custom function name.
     * @param {Function} fn Custom function.
     * @returns {Parser}
     */

  }, {
    key: "setFunction",
    value: function setFunction(name, fn) {
      this.functions[name] = fn;
      return this;
    }
    /**
     * Get custom function.
     *
     * @param {String} name Custom function name.
     * @returns {*}
     */

  }, {
    key: "getFunction",
    value: function getFunction(name) {
      return this.functions[name];
    }
    /**
     * Call function with provided params.
     *
     * @param name Function name.
     * @param params Function params.
     * @returns {*}
     * @private
     */

  }, {
    key: "_callFunction",
    value: function _callFunction(name) {
      var params = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : [];
      var fn = this.getFunction(name);
      var value;

      if (fn) {
        value = fn(params);
      }

      this.emit('callFunction', name, params, function (newValue) {
        if (newValue !== void 0) {
          value = newValue;
        }
      });
      return value === void 0 ? (0, _evaluateByOperator["default"])(name, params) : value;
    }
    /**
     * Retrieve value by its label (`B3`, `B$3`, `B$3`, `$B$3`).
     *
     * @param {String} label Coordinates.
     * @returns {*}
     * @private
     */

  }, {
    key: "_callCellValue",
    value: function _callCellValue(label) {
      label = label.toUpperCase();

      var _extractLabel = (0, _cell.extractLabel)(label),
          _extractLabel2 = _slicedToArray(_extractLabel, 2),
          row = _extractLabel2[0],
          column = _extractLabel2[1];

      var value = void 0;
      this.emit('callCellValue', {
        label: label,
        row: row,
        column: column
      }, function (_value) {
        value = _value;
      });
      return value;
    }
    /**
     * Retrieve value by its label (`B3:A1`, `B$3:A1`, `B$3:$A1`, `$B$3:A$1`).
     *
     * @param {String} startLabel Coordinates of the first cell.
     * @param {String} endLabel Coordinates of the last cell.
     * @returns {Array} Returns an array of mixed values.
     * @private
     */

  }, {
    key: "_callRangeValue",
    value: function _callRangeValue(startLabel, endLabel) {
      startLabel = startLabel.toUpperCase();
      endLabel = endLabel.toUpperCase();

      var _extractLabel3 = (0, _cell.extractLabel)(startLabel),
          _extractLabel4 = _slicedToArray(_extractLabel3, 2),
          startRow = _extractLabel4[0],
          startColumn = _extractLabel4[1];

      var _extractLabel5 = (0, _cell.extractLabel)(endLabel),
          _extractLabel6 = _slicedToArray(_extractLabel5, 2),
          endRow = _extractLabel6[0],
          endColumn = _extractLabel6[1];

      var startCell = {};
      var endCell = {};

      if (startRow.index <= endRow.index) {
        startCell.row = startRow;
        endCell.row = endRow;
      } else {
        startCell.row = endRow;
        endCell.row = startRow;
      }

      if (startColumn.index <= endColumn.index) {
        startCell.column = startColumn;
        endCell.column = endColumn;
      } else {
        startCell.column = endColumn;
        endCell.column = startColumn;
      }

      startCell.label = (0, _cell.toLabel)(startCell.row, startCell.column);
      endCell.label = (0, _cell.toLabel)(endCell.row, endCell.column);
      var value = [];
      this.emit('callRangeValue', startCell, endCell, function () {
        var _value = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : [];

        value = _value;
      });
      return value;
    }
    /**
     * Try to throw error by its name.
     *
     * @param {String} errorName Error name.
     * @returns {String}
     * @private
     */

  }, {
    key: "_throwError",
    value: function _throwError(errorName) {
      if ((0, _error.isValidStrict)(errorName)) {
        throw Error(errorName);
      }

      throw Error(_error.ERROR);
    }
  }]);

  return Parser;
}(_tinyEmitter["default"]);

var _default = Parser;
exports["default"] = _default;