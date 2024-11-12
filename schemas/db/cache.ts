import { sqliteTable, integer, blob, text } from "drizzle-orm/sqlite-core"

//Persistent cache of resources which are not locally stored, to avoid excessive delays.
export const cache = sqliteTable('cache', {
  id: integer().primaryKey({autoIncrement:true}),
  //The 256bit key used to uniquely identify a certain application class. 0 is left for the current user.
  hash: blob({mode:'buffer'}).unique().notNull(),
  //Timestamp of when it was taken
  timestamp_creation: integer().notNull(),
  //Timestamp of when it expires
  timestamp_expiring: integer().notNull(),
  //URL of the resource
  source: text().notNull(),
  //Data
  content: blob().notNull(),
});