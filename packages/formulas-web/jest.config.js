module.exports = {
  testEnvironment: "jsdom",
  testMatch: ["<rootDir>/tests/**/*.test.js"],
  testTimeout: 30000,
  setupFilesAfterEnv: ["<rootDir>/jest.setup.js"],
  transform: {},
  moduleNameMapper: {
    "^(\\.{1,2}/.*)\\.js$": "$1"
  }
};
