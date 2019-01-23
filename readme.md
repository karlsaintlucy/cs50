# CS50
#### Karl Saint Lucy's coursework for HarvardX's CS50 (Introduction to Computer Science) course.

I completed [CS50](https://www.edx.org/course/cs50s-introduction-computer-science-harvardx-cs50x) in February of 2018, and my validated certificate of achievement is available to view [here](https://courses.edx.org/certificates/020a0ef7eed84154ace7a3fd07902f46). Course materials involved programming in C, JavaScript, and Python, and front-end frameworks like jQuery, HTML, and CSS.

### edX Honor Code
This material demonstrates my personal achievements in a MOOC owned and operated by Harvard University on the edX platform. Its availability does not imply my ownership of the curriculum materials, nor does it constitute permission to plagiarize my coursework.

My understanding is that this publication of my work falls under Fair Use and does not violate edX's [Terms of Service and Honor Code](https://www.edx.org/edx-terms-service) — if you are an owner or operator of CS50 and disagree, please write me at karl [at] karlsaintlucy.com and I will be glad to remove it.

### Notes on excelerateAR
Please note that my final project, **excelerateAR**, contains proprietary code owned by Idealist.org and is contained in a private repository. This was in fulfillment of CS50's directive that the final project "solve a real problem in the real world."

#### About

**excelerateAR** is a Python command-line interface application that, given a list of Idealist.org clients (listed in a plain-text file and separated by newlines), queries Idealist's production PostgreSQL database and aggregates outstanding invoice data from the client accounts in question. It then creates an Excel spreadsheet for each client in question and writes detail on all past-due invoices in that spreadsheet.

I wrote this application when I served as Idealist's Operations Coordinator, which involved running Accounts Receivables campaigns.

#### User Story

When Idealist launched the minimum-viable-product v7 of its job listings platform, the platform temporarily lost its ability to automatically send invoices and invoice notifications to clients. Idealist had never previously been capable of generating Excel statements with detail on all outstanding invoices for clients. Rather, clients were only able to view invoice details one invoice at a time (e.g., clients could never see a straightforward "total balance" on their client dashboards).

I built **excelerateAR** to:

* Bridge the gap between MVP and mission-critical Accounts Receivables platform features;
* Improve client experience by allowing a bird's-eye-view of all outstanding invoices and total balance; 
* Be easy to use for non-technical users; and
* Fulfill the final project requirements for CS50.

_When implemented, **excelerateAR** was instrumental in closing $50,000 in outstanding Accounts Receivables in two weeks._

#### Features & Modules
**excelerateAR** uses the following features:

* Command-line interface
* Low-level database access using raw PostgreSQL queries with the `psycopg2` API
* Customizable preferences available in a `preferences.json` file
* Logging
* Writing to Excel using the `xlsxwriter` module
* Directory structure that creates a new folder tree for every execution of the application and separates Excel statements by client