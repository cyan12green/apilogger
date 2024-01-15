module.exports = {
  resolve: {
    fallback: { "crypto": require.resolve("crypto-browserify") }
  },
  mode: "production",
  entry: "./api_logger_client.js",
  target: "node",
  node: {
    __dirname: false,
  },
  module: {
    rules: [
      {
        test: /\.node$/,
        loader: "node-loader",
      },
    ],
  },
};
