describe('XL Formula Integration Tests', () => {
  let XLFormulaModule;
  let engine;

  beforeAll(async () => {
    // Wait for global WASM initialization to complete
    await global.wasmInitializationPromise;
    
    if (!global.hasWasmAvailable()) {
      throw new Error('WASM not available');
    }

    // Get the globally initialized instances
    XLFormulaModule = global.getXLFormulaModule();
    engine = global.getFormulaEngine();
  });

  describe('Basic Functionality', () => {
    test('should initialize without errors', () => {
      expect(XLFormulaModule).toBeDefined();
      expect(engine).toBeDefined();
      expect(typeof engine.evaluateText).toBe('function');
      expect(typeof engine.evaluateNumber).toBe('function');
      expect(typeof engine.evaluateBoolean).toBe('function');
    });
  });

  describe('Math Functions', () => {
    test('SUM() should add numbers correctly', () => {
      const result = engine.evaluateNumber('SUM(1, 2, 3)');
      console.log('SUM(1, 2, 3) result:', result);
      
      expect(typeof result).toBe('number');
      expect(result).toBe(6);
    });

    test('MAX() should find maximum correctly', () => {
      const result = engine.evaluateNumber('MAX(10, 20, 5)');
      console.log('MAX(10, 20, 5) result:', result);
      
      expect(typeof result).toBe('number');
      expect(result).toBe(20);
    });

    test('AVERAGE() should calculate average correctly', () => {
      const result = engine.evaluateNumber('AVERAGE(1, 2, 3, 4, 5)');
      console.log('AVERAGE(1, 2, 3, 4, 5) result:', result);
      
      expect(typeof result).toBe('number');
      expect(result).toBe(3);
    });
  });

  describe('DateTime Functions - Critical Test', () => {
    test('NOW() should return a valid date string', () => {
      const result = engine.evaluateText('NOW()');
      console.log('🔍 NOW() result:', result, 'Type:', typeof result);
      
      expect(typeof result).toBe('string');
      // Should NOT be "0" - this would indicate the bug
      expect(result).not.toBe('0');
      // Should match date pattern YYYY-MM-DD HH:MM:SS
      expect(result).toMatch(/^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$/);
    });

    test('TODAY() should return a valid date string', () => {
      const result = engine.evaluateText('TODAY()');
      console.log('🔍 TODAY() result:', result, 'Type:', typeof result);
      
      expect(typeof result).toBe('string');
      // Should NOT be "0" - this would indicate the bug
      expect(result).not.toBe('0');
      // Should match date pattern YYYY-MM-DD HH:MM:SS (with midnight time)
      expect(result).toMatch(/^\d{4}-\d{2}-\d{2} 00:00:00$/);
    });

    test('DATE() should create valid dates', () => {
      const result = engine.evaluateText('DATE(2024, 1, 15)');
      console.log('🔍 DATE(2024, 1, 15) result:', result, 'Type:', typeof result);
      
      expect(typeof result).toBe('string');
      // Should NOT be "0" - this would indicate the bug
      expect(result).not.toBe('0');
      // Should be a valid date string with midnight time
      expect(result).toMatch(/^\d{4}-\d{2}-\d{2} 00:00:00$/);
    });

    test('YEAR() should extract year correctly', () => {
      const result = engine.evaluateNumber('YEAR(DATE(2024, 1, 15))');
      console.log('🔍 YEAR(DATE(2024, 1, 15)) result:', result, 'Type:', typeof result);
      
      expect(typeof result).toBe('number');
      // Should NOT be 0 - this would indicate the bug
      expect(result).not.toBe(0);
      expect(result).toBe(2024);
    });

    test('MONTH() should extract month correctly', () => {
      const result = engine.evaluateNumber('MONTH(DATE(2024, 3, 15))');
      console.log('🔍 MONTH(DATE(2024, 3, 15)) result:', result, 'Type:', typeof result);
      
      expect(typeof result).toBe('number');
      // Should NOT be 0 - this would indicate the bug
      expect(result).not.toBe(0);
      expect(result).toBe(3);
    });

    test('DAY() should extract day correctly', () => {
      const result = engine.evaluateNumber('DAY(DATE(2024, 1, 15))');
      console.log('🔍 DAY(DATE(2024, 1, 15)) result:', result, 'Type:', typeof result);
      
      expect(typeof result).toBe('number');
      // Should NOT be 0 - this would indicate the bug
      expect(result).not.toBe(0);
      expect(result).toBe(15);
    });

    test('NOW() should return a JavaScript Date object', () => {
      const result = engine.evaluateDate('NOW()');
      console.log('🔍 NOW() as Date:', result, 'Type:', typeof result);
      
      expect(result).toBeInstanceOf(Date);
      expect(result.getTime()).toBeGreaterThan(0);
    });

    test('TODAY() should return a JavaScript Date object', () => {
      const result = engine.evaluateDate('TODAY()');
      console.log('🔍 TODAY() as Date:', result, 'Type:', typeof result);
      
      expect(result).toBeInstanceOf(Date);
      expect(result.getTime()).toBeGreaterThan(0);
      // Should be midnight (00:00:00)
      expect(result.getHours()).toBe(0);
      expect(result.getMinutes()).toBe(0);
      expect(result.getSeconds()).toBe(0);
    });

    test('DATE() should return a JavaScript Date object', () => {
      const result = engine.evaluateDate('DATE(2024, 1, 15)');
      console.log('🔍 DATE(2024, 1, 15) as Date:', result, 'Type:', typeof result);
      
      expect(result).toBeInstanceOf(Date);
      expect(result.getFullYear()).toBe(2024);
      expect(result.getMonth()).toBe(0); // January is 0
      expect(result.getDate()).toBe(15);
      // Should be midnight (00:00:00)
      expect(result.getHours()).toBe(0);
      expect(result.getMinutes()).toBe(0);
      expect(result.getSeconds()).toBe(0);
    });
  });

  describe('Text Functions', () => {
    test('CONCATENATE() should join strings correctly', () => {
      const result = engine.evaluateText('CONCATENATE("Hello", " ", "World")');
      console.log('CONCATENATE("Hello", " ", "World") result:', result);
      
      expect(typeof result).toBe('string');
      expect(result).toBe('Hello World');
    });

    test('LEN() should return correct string length', () => {
      const result = engine.evaluateNumber('LEN("Hello World")');
      console.log('LEN("Hello World") result:', result);
      
      expect(typeof result).toBe('number');
      expect(result).toBe(11);
    });
  });

  describe('Logical Functions', () => {
    test('IF() should handle conditional logic correctly', () => {
      const result1 = engine.evaluateText('IF(TRUE, "Yes", "No")');
      const result2 = engine.evaluateText('IF(FALSE, "Yes", "No")');
      console.log('IF(TRUE, "Yes", "No") result:', result1);
      console.log('IF(FALSE, "Yes", "No") result:', result2);
      
      expect(typeof result1).toBe('string');
      expect(result1).toBe('Yes');
      expect(typeof result2).toBe('string');
      expect(result2).toBe('No');
    });

    test('AND() should handle logical AND correctly', () => {
      const result1 = engine.evaluateBoolean('AND(TRUE, TRUE)');
      const result2 = engine.evaluateBoolean('AND(TRUE, FALSE)');
      console.log('AND(TRUE, TRUE) result:', result1);
      console.log('AND(TRUE, FALSE) result:', result2);
      
      expect(typeof result1).toBe('boolean');
      expect(result1).toBe(true);
      expect(typeof result2).toBe('boolean');
      expect(result2).toBe(false);
    });
  });
});