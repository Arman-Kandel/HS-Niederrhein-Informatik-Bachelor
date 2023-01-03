#!/bin/bash
# Test locally, note that postgres must have no password:
psql -U postgres -h /var/run/postgresql -p 5432 -d postgres -c "\i p3.sql"
