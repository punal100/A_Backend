import axios from 'axios';

// CHECK: Ensure this port matches your main.cpp (usually 18080 for Crow)
// Use environment variables for baseURL to allow for different environments (dev, prod).
// VITE_API_URL would be defined in .env files (e.g., .env.development, .env.production)
const instance = axios.create({
  baseURL: import.meta.env.VITE_API_URL || 'http://localhost:18080/api',
  // Increased timeout to 5 seconds for more reliability on slower networks.
  timeout: 5000,
  // Removed placeholder custom header. Add back any headers your API actually requires.
  headers: {}
});

export default instance;
