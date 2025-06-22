# Water Quality Dashboard

## Overview
The Water Quality Dashboard is a web application designed to monitor and display water quality metrics such as pH, turbidity, TDS (Total Dissolved Solids), and temperature. The application provides a user-friendly interface for users to log in, sign up, and view water quality data.

## Features
- User authentication with login and sign-up functionality.
- A dashboard displaying key water quality metrics.
- A "Forgot Password" feature for user account recovery.

## Project Structure
```
water-quality-dashboard
├── public
│   └── index.html          # Main HTML entry point
├── src
│   ├── components          # React components
│   │   ├── Dashboard.tsx
│   │   ├── Login.tsx
│   │   ├── Signup.tsx
│   │   ├── Welcome.tsx
│   │   └── ForgotPassword.tsx
│   ├── pages               # Page components
│   │   ├── DashboardPage.tsx
│   │   ├── LoginPage.tsx
│   │   ├── SignupPage.tsx
│   │   └── WelcomePage.tsx
│   ├── App.tsx             # Main application component
│   ├── index.tsx           # Entry point for the React application
│   └── types               # TypeScript interfaces
│       └── index.ts
├── package.json            # npm configuration file
├── tsconfig.json           # TypeScript configuration file
└── README.md               # Project documentation
```

## Setup Instructions
1. Clone the repository:
   ```
   git clone <repository-url>
   cd water-quality-dashboard
   ```

2. Install dependencies:
   ```
   npm install
   ```

3. Start the development server:
   ```
   npm start
   ```

4. Open your browser and navigate to `http://localhost:3000` to view the application.

## Usage
- On the welcome page, users can log in or sign up for a new account.
- After logging in, users will be directed to the dashboard where they can view the water quality metrics.
- If users forget their password, they can use the "Forgot Password" feature to reset it.

## Contributing
Contributions are welcome! Please submit a pull request or open an issue for any enhancements or bug fixes.

## License
This project is licensed under the MIT License.