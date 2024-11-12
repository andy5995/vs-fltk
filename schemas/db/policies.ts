import { sqliteTable, integer, blob, text } from "drizzle-orm/sqlite-core"

export const policies = sqliteTable('policies', {
  id: integer().primaryKey({autoIncrement:true}),
  //The 256bit key used to uniquely identify a certain application class. 0 is left for the current user.
  app_id: blob({mode:'buffer'}).unique(),
  //TODO
  data: text().notNull()
});