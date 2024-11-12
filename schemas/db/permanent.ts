import { sqliteTable, integer, uniqueIndex, text, blob } from "drizzle-orm/sqlite-core"
import { apps } from "./apps";

export const permanent = sqliteTable('permanent', {
  id: integer().primaryKey(),
  app: integer().references(()=> apps.id, {onDelete:'cascade',onUpdate:'cascade'}).notNull(),
  key: text().notNull(),
  value: blob().notNull(),
},(table)=>({idx:uniqueIndex('permanentUniqueIdx').on(table.app,table.key)}));